#include <fft.h>

#include <fftw3.h>
#include <cassert>
#include <cmath>
#include <algorithm>

std::size_t N;
ld *fft_in;
fftwl_complex *fft_out;
fftwl_plan fft_plan;

void init_fft(std::size_t size) {
    N = size;

    fft_in  =    fftwl_alloc_real(N);
    fft_out = fftwl_alloc_complex(N);

    fft_plan = fftwl_plan_dft_r2c_1d(
        N, fft_in, fft_out,
        FFTW_FORWARD | FFTW_MEASURE
    );
}

void kill_fft(void) {
    fftwl_destroy_plan(fft_plan);
    fftwl_free(fft_in), fftwl_free(fft_out);
}

void fft(const std::vector<data_point_t> &in, std::vector<wave_t> &out) {
    assert(in.size() == N);

    for(std::size_t i = 0; i < N; ++i)
        fft_in[i] = in[i].velocity;

    fftwl_execute(fft_plan);

    assert(fft_out[0][1] == 0);

    out.clear();

    for(std::size_t i = 0; i < N; ++i) {
        auto [real, imaginary] = fft_out[i];

        out.push_back((wave_t){
            (i * SAMPLE_RATE) / N,
            sqrtl(real * real + imaginary * imaginary),
            atanl(imaginary / real)
        });
    }

    std::sort(out.begin(), out.end(), [&](const wave_t &a, const wave_t &b) {
        return a.amplitude > b.amplitude;
    });

    out.resize(FFT_OUT_APPROX);
}