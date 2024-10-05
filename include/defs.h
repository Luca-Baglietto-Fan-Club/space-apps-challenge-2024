#ifndef __SAC24_DEFS_H
#define __SAC24_DEFS_H

#include <cstddef>

using ld = long double;

constexpr std::size_t DATAPOINTS_PER_12H = 4320;
constexpr std::size_t FFT_OUT_APPROX = 32;
constexpr ld SAMPLE_RATE = 0.1l;

typedef struct {
    float time;
    ld velocity;
} data_point_t;

typedef struct {
    ld frequency;
    ld amplitude;
} wave_t;

#endif /* __SAC24_DEFS_H */