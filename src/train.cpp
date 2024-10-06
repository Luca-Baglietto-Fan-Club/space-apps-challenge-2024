#include <pre.h>
#include <fft.h>
#include <k_means.h>

#include <vector>

#include <cstdio>
#include <iostream>

constexpr std::size_t DATA_COUNT = 152;

int main(void) {
    std::vector<std::vector<data_point_t>> csv12(DATA_COUNT);
    for(std::size_t i = 0; i < DATA_COUNT; ++i)
        parse12(csv12[i]);

    init_fft(DATAPOINTS_PER_12H);

    std::vector<std::vector<data_point_t>> fft_input(DATA_COUNT - 1);
    for(std::size_t i = 0; i < DATA_COUNT - 1; ++i) {
        for(auto item: csv12[i])     fft_input[i].push_back(item);
        for(auto item: csv12[i + 1]) fft_input[i].push_back(item);
    }

    csv12.clear();

    std::vector<std::vector<wave_t>> fft_output(DATA_COUNT - 1);
    for(std::size_t i = 0; i < DATA_COUNT - 1; ++i)
        fft(fft_input[i], fft_output[i]);

    fft_input.clear();

    std::freopen("out", "w", stdout);

    for(std::size_t i = 0; i < DATA_COUNT - 1; ++i) {
        for(auto [frequency, amplitude, phase]: fft_output[i]) {
            std::cout << "F: " << frequency <<
                "; A: " << amplitude <<
                "; P: " << phase <<
                std::endl;
        }

        std::cout << "=======================" << std::endl;
    }    

    kill_fft();
    return 0;
}