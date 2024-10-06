#ifndef __SAC24_DEFS_H
#define __SAC24_DEFS_H

#include <cstddef>

using ld = long double;

constexpr std::size_t FFT_OUT_APPROX = 32;
constexpr std::size_t DATA_POINT_PER_BLOCK = 10800;
constexpr std::size_t DATA_BLOCKS = 8;
constexpr ld DATA_POINT_FREQ = 1.0l;

typedef struct {
    float time;
    ld velocity;
} data_point_t;

typedef struct {
    ld frequency;
    ld amplitude;
    ld phase;
} wave_t;

#endif /* __SAC24_DEFS_H */