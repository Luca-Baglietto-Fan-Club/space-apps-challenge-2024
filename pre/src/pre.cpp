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
    
    for(std::size_t i = 0; i < pv_cnt; i++)
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
        if(i-pv_cnt >= 0)
        {
            dyncount -= fabs(data[i-pv_cnt].velocity);
            r_size--;
        } 
        temp[i].time = data[i].time;
        temp[i].velocity = dyncount/(2*r_size) * (data[i].velocity < 0 ? -1 : 1);
    }

    swap(data, temp);
}

std::size_t bin_search(rapidcsv::Document &doc, float target)
{
    std::size_t l = 0, r = doc.GetRowCount()-1;

    while(l < r)
    {
        std::size_t mid = (l+r)/2;
        if(doc.GetCell<float>(1, mid) < target) l = mid;
        else r = mid;
    }

    return l;
}

std::size_t parse12(std::vector<data_point_t> &out) {
    static std::size_t it = 0;

    rapidcsv::Document doc(std::to_string((it/2)) + ".csv", rapidcsv::LabelParams(0, -1));
    std::size_t st_point = ((it&1) ? bin_search(doc, 43200) : 0);

    std::size_t DATA_POINTS = doc.GetRowCount()-st_point;

    out.resize(DATA_POINTS-st_point);
    long double DATA_AVERAGE = 0;
    long double ABS_MIN = 0x3f3f3f3f;

    
    //DATA PARSING
    for(std::size_t i = st_point; i < DATA_POINTS; i++)
    {
        std::vector<long double> c = doc.GetRow<long double>(i);

        data_point_t d;

        d.time = c[1];
        d.velocity = c[2];

        DATA_AVERAGE += fabs(c[2]);
        ABS_MIN = std::min(ABS_MIN, (long double)fabs(c[2]));
    }
    DATA_AVERAGE /= (long double)DATA_POINTS;


    //SMOOTHING FILTER
    for(std::size_t _ = 0; _ < SMOOTHING_PASS; _++)
        __smoothing_func(out, SMOOTHING_RANGE);



    //HIGH PASS FILTER
    // long double HIGH_PASS_THRESHOLD = ABS_MIN + (DATA_AVERAGE-ABS_MIN)*THRESHOLD_FACTOR;
    // __high_pass(out, HIGH_PASS_THRESHOLD);




    // WRITE DATA IN BLOCKS.CSV in 43500 blocks
    // TODO:    pass this values to the FTT function
    //          as structs.

    // int lk = 0;
    // int lt = 0;
    // int it = 0;
    // while(lk < DATA_POINTS)
    // {
    //     std::ofstream block("block" + std::to_string(it) + ".csv");
    //     block << "time_abs(%Y-%m-%dT%H:%M:%S.%f),time_rel(sec),velocity(m/s)\n";

    //     while(lk < DATA_POINTS && out[lk][0] < (long double) (lt+43500))
    //     {
    //         block << doc.GetCell<std::string>(0, lk) << ", " <<  out[lk][0] << ", " <<  out[lk][1] << "\n";
    //         lk++;
    //     }
    //     lt = out[lk][0];
    //     it++;
    //     block.close();
    // }

    it++;
    return it-1;
}