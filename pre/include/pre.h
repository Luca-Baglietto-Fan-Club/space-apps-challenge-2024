#ifndef __SAC24_PRE_H
#define __SAC24_PRE_H

#include <defs.h>
#include <cstddef>
#include <vector>

// HIGH PASS FILTER
// constexpr long double THRESHOLD_FACTOR = 0.2;

// SMOOTHING FILTER
constexpr int SMOOTHING_PASS = 1;
constexpr int SMOOTHING_RANGE = 500;


std::size_t parse12(std::vector<data_point_t> &out);

#endif /* __SAC24_PRE_H */