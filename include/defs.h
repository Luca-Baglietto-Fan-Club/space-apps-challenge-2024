#ifndef __SAC24_DEFS_H
#define __SAC24_DEFS_H

#include <cstddef>

using ld = long double;

constexpr std::size_t DATAPOINTS_PER_12H = 4320;
constexpr std::size_t FFT_OUT_APPROX = 32;
constexpr ld SAMPLE_RATE = 0.1l;


// 8 data blocks implies that each block
// will have 24/8 = 3 hours of data
constexpr std::size_t DATA_BLOCKS = 8;

// Same as SAMPLE_RATE
constexpr ld DATA_POINT_FREQ = 1.0l;

// Same as DATAPOINTS_PER_12H
constexpr std::size_t DATA_POINT_PER_BLOCK = 10800;

typedef struct {
    float time;
    ld velocity;
} data_point_t;

typedef struct {
    ld frequency;
    ld amplitude;
} wave_t;

#endif /* __SAC24_DEFS_H */