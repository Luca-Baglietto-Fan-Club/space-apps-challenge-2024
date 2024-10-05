#ifndef K_MEANS_H
#define K_MEANS_H

#include <vector>
#include <defs.h>

struct trained_k_means_algo_t {
    std::vector<long double> centr1;
    std::vector<long double> centr2;
};

trained_k_means_algo_t train_k_means(std::vector<long double> &seed1, std::vector<long double> &seed2);

#endif