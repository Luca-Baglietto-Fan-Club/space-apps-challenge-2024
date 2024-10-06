#include <pre.h>
#include <cstddef>
#include <string>
#include <rapidcsv.h>


// void __high_pass(std::vector<std::array<long double, 2>> &data, long double threshold, int col = 1)
// {
//     for(auto &i : data)
//     {
//         i[col] = (fabs(i[col]) < threshold) ? 0 : i[col];
//     }
// }

// std::size_t bin_search(rapidcsv::Document &doc, float target)
// {
//     std::size_t l = 0, r = doc.GetRowCount()-1;

//     while(l < r)
//     {
//         std::size_t mid = (l+r)/2;
//         if(doc.GetCell<float>(1, mid) < target) l = mid+1;
//         else r = mid;
//     }

//     return l;
// }

void __smoothing_func(std::vector<data_point_t> &data, std::size_t pv_cnt)
{

    //NOTE: this could be optimized by keeping the last "pv_cnt" values 
    //      in a FIFO struct and updating "data" as the cycles progresses

    std::size_t dataset_size = data.size();
    std::vector<data_point_t> temp(dataset_size);


    // Range size
    long double r_size = 0;

    // Range sum value
    long double dyncount = 0;
    
    for(std::size_t i = 0; i < pv_cnt && i < dataset_size; i++)
    {
        dyncount += fabs(data[i].velocity);
        r_size++;
    }

    for(std::size_t i = 0; i < dataset_size; i++)
    {
        if(i+pv_cnt < dataset_size)
        {
            dyncount += fabs(data[i+pv_cnt].velocity);
            r_size++;
        } 

        //  needs (int) casting or it will underflow
        //  instead of using i-pv_cnt >= 0, use i >= pv_cnt
        if(i >= pv_cnt)
        {
            dyncount -= fabs(data[i-pv_cnt].velocity);
            r_size--;
        } 
        temp[i].time = data[i].time;

        temp[i].velocity = dyncount/(2*r_size) * (data[i].velocity < 0 ? -1 : 1);
    }

    swap(data, temp);
}


std::size_t parse12(std::vector<std::vector<data_point_t>> &out) {
    static std::size_t it = 0;

    //open (#it).csv
    rapidcsv::Document doc(std::to_string(it) + ".csv", rapidcsv::LabelParams(0, -1));
    std::size_t CSV_LENGTH = (std::size_t) doc.GetRowCount();

    //Smoothing statistics
    //Smoothing statistics
    long double DATA_AVERAGE = 0;
    long double ABS_MIN = 0x3f3f3f3f;

    //DATA PARSING
    bool alt_negative = 0;
    long double mean = 0;
    std::size_t dp_size = 0;

    out.resize(DATA_BLOCKS);
    std::size_t currentBlock = 0;

    float increment = 1.0/DATA_POINT_FREQ;
    float c_target = increment;

    for(std::size_t i = 0; i < CSV_LENGTH && currentBlock < DATA_BLOCKS; i++)
    {

        std::vector<long double> c = doc.GetRow<long double>(i);

        mean += 1.0/c[2];
        dp_size++;
        
        //if datapoint delta > target
        
        //if datapoint delta > target
        if(c[1] >= c_target)
        {   

            data_point_t d;
            d.time = c_target;


            assert(mean != 0);
            d.velocity = (long double)dp_size/mean;

            out[currentBlock].push_back(d);

            c_target += increment;
            alt_negative = !alt_negative;
            mean = 0;
            dp_size = 0;

            if(out[currentBlock].size() >= DATA_POINT_PER_BLOCK)
                currentBlock++;
        }

        DATA_AVERAGE += fabs(c[2]);
        ABS_MIN = std::min(ABS_MIN, (long double)fabs(c[2]));
    }

    DATA_AVERAGE /= (long double)CSV_LENGTH;

    //SMOOTHING FILTER
    for(auto &block : out)
        for(std::size_t _ = 0; _ < SMOOTHING_PASS; _++)
        __smoothing_func(block, SMOOTHING_RANGE);

    //HIGH PASS FILTER
    // long double HIGH_PASS_THRESHOLD = ABS_MIN + (DATA_AVERAGE-ABS_MIN)*THRESHOLD_FACTOR;
    // __high_pass(out, HIGH_PASS_THRESHOLD);

    // Guarantees blocks are of correct size
    assert(out.size() == DATA_BLOCKS);
    for(auto &i : out)
    {
        assert(i.size() >= DATA_POINT_PER_BLOCK);
        i.resize(DATA_POINT_PER_BLOCK);
    }

    it++;
    return it-1;
}