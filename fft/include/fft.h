#ifndef __SAC24_FFT_H
#define __SAC24_FFT_H

#include <defs.h>
#include <vector>

void init_fft(std::size_t size);
void kill_fft(void);

void fft(const std::vector<data_point_t> &in, std::vector<wave_t> &out);

#endif /* __SAC24_FFT_H */