#include <fft.h>

#include <fftw3.h>
#include <cassert>
#include <cmath>

ld *fft_in;
fftwl_complex *fft_out;
fftwl_plan fft_plan;

void init_fft(void) {
    fft_in  =    fftwl_alloc_real(DATAPOINTS_PER_12H);
    fft_out = fftwl_alloc_complex(DATAPOINTS_PER_12H);

    fft_plan = fftwl_plan_dft_r2c_1d(
        DATAPOINTS_PER_12H,
        fft_in, fft_out,
        FFTW_FORWARD | FFTW_MEASURE
    );
}

void kill_fft(void) {
    fftwl_destroy_plan(fft_plan);
    fftwl_free(fft_in), fftwl_free(fft_out);
}

void fft(std::vector<data_point_t> &in, std::vector<wave_t> &out) {
    assert(in.size() == DATAPOINTS_PER_12H);

    for(std::size_t i = 0; i < DATAPOINTS_PER_12H; ++i)
        fft_in[i] = in[i].velocity;

    fftwl_execute(fft_plan);

    assert(fft_out[0][1] == 0);

    out.clear();

    std::size_t delta = DATAPOINTS_PER_12H - FFT_OUT_APPROX;
    for(std::size_t i = 0; i < FFT_OUT_APPROX; ++i) {
        auto [real, imaginary] = fft_out[i + delta];

        out.push_back((wave_t){
            (i * SAMPLE_RATE) / DATAPOINTS_PER_12H,
            sqrtl(real * real + imaginary * imaginary)
        });
    }
}