#include <rapidcsv.h>
#include <iostream>

// void printData(std::vector<std::array<long double, 2>> &data)
// {
//     for(auto i : data)
//     {
//         std::cout << i[0] << " " << i[1] << "\n";
//     }
// }
// void printCsv(std::vector<std::array<long double, 2>> &data)
// {
//     std::ofstream dumper("processed.csv");
//     dumper << "time_abs(%Y-%m-%dT%H:%M:%S.%f),time_rel(sec),velocity(m/s)\n";
//     for(auto i : data)
//     {
//         dumper << "invalid, " << i[0] << ", " << i[1] << "\n";
//     }
// }   
// void debug(long double value)
// {
//     std::cout << "DBG: " << value << "\n";
// }


void __high_pass(std::vector<std::array<long double, 2>> &data, long double threshold, int col = 1)
{
    for(auto &i : data)
    {
        i[col] = (fabs(i[col]) < threshold) ? 0 : i[col];
    }
}

void __smoothing_func(std::vector<std::array<long double, 2>> &data, int pv_cnt, int col = 1)
{

    //NOTE: this could be optimized by keeping the last "pv_cnt" values 
    //      in a FIFO struct and updating "data" as the cycles progresses

    long double dyncount = 0;
    int dataset_size = data.size();
    std::vector<std::array<long double, 2>> temp(dataset_size);
    long double r_size = 0;
    for(int i = 0; i < pv_cnt; i++)
    {
        dyncount += fabs(data[i][col]);
        r_size++;
    }

    for(int i = 0; i < dataset_size; i++)
    {
        if(i+pv_cnt < dataset_size)
        {
            dyncount += fabs(data[i+pv_cnt][col]);
            r_size++;
        } 
        if(i-pv_cnt >= 0)
        {
            dyncount -= fabs(data[i-pv_cnt][col]);
            r_size--;
        } 

        // std::cout << dyncount << " " << r_size << "\n";
        temp[i] = data[i];
        temp[i][col] = dyncount/(2*r_size) * (data[i][col] < 0 ? -1 : 1);
    }

    swap(data, temp);
}




int main(int argc, char* argv[])
{
    // HIGH PASS FILTER
    // constexpr long double THRESHOLD_FACTOR = 0.2;

    // SMOOTHING FILTER
    constexpr int SMOOTHING_PASS = 1;
    constexpr int SMOOTHING_RANGE = 500;


    if(argc != 2)
    {
        //sussato
        std::cout << "Invalid usage\n";
        return 1;
    }

    rapidcsv::Document doc(argv[1], rapidcsv::LabelParams(0, -1));

    int DATA_POINTS = doc.GetRowCount();
    std::vector<std::array<long double, 2>> dp(DATA_POINTS);
    long double DATA_AVERAGE = 0;
    long double ABS_MIN = 0x3f3f3f3f;

    
    //DATA PARSING
    for(int i = 0; i < DATA_POINTS; i++)
    {
        std::vector<long double> c = doc.GetRow<long double>(i);
        dp[i][0] = c[1];
        dp[i][1] = c[2];

        DATA_AVERAGE += fabs(c[2]);
        ABS_MIN = std::min(ABS_MIN, (long double)fabs(c[2]));
    }
    DATA_AVERAGE /= (long double)DATA_POINTS;


    //SMOOTHING FILTER
    for(int _ = 0; _ < SMOOTHING_PASS; _++)
        __smoothing_func(dp, SMOOTHING_RANGE);

    //HIGH PASS FILTER
    // long double HIGH_PASS_THRESHOLD = ABS_MIN + (DATA_AVERAGE-ABS_MIN)*THRESHOLD_FACTOR;
    // __high_pass(dp, HIGH_PASS_THRESHOLD);

    //Print data in 43500 blocks
    // TODO:    pass this values to the FTT function
    //          as structs.

    int lk = 0;
    int lt = 0;
    int it = 0;
    while(lk < DATA_POINTS)
    {
        std::ofstream block("block" + std::to_string(it) + ".csv");
        block << "time_abs(%Y-%m-%dT%H:%M:%S.%f),time_rel(sec),velocity(m/s)\n";

        while(lk < DATA_POINTS && dp[lk][0] < (long double) (lt+43500))
        {
            block << doc.GetCell<std::string>(0, lk) << ", " <<  dp[lk][0] << ", " <<  dp[lk][1] << "\n";
            lk++;
        }
        lt = dp[lk][0];
        it++;
        block.close();
    }
    return 0;
}