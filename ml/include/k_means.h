#ifndef __SAC24_K_MEANS_H
#define __SAC24_K_MEANS_H

#include <vector>
#include <defs.h>

struct trained_k_means_algo_t {
    std::vector<ld> centr1;
    std::vector<ld> centr2;
};

trained_k_means_algo_t train_k_means(std::vector<ld> &seed1,
   std::vector<ld> &seed2, std::vector<std::vector<wave_t>> &data);
   
bool k_means_query(trained_k_means_algo_t &train_data, std::vector<ld> &queryPoint);

#endif