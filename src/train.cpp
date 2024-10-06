#include <pre.h>
#include <fft.h>
#include <k_means.h>

#include <vector>

#include <cstdio>
#include <iostream>

constexpr std::size_t CSV_COUNT = 76;

int main(void) {
    std::vector<std::vector<std::vector<data_point_t>>> csv(CSV_COUNT);
    for(auto &csv_block: csv) parse(csv_block);

    init_fft(DATA_POINT_PER_BLOCK);

    std::vector<std::vector<data_point_t>> fft_input(CSV_COUNT * DATA_BLOCKS);
    for(std::size_t i = 0; i < CSV_COUNT; ++i)
        for(std::size_t j = 0; j < DATA_BLOCKS; ++j)
            fft_input[i * DATA_BLOCKS + j] = csv[i][j];

    csv.clear();

    std::vector<std::vector<wave_t>> fft_output(fft_input.size());
    for(std::size_t i = 0; i < fft_input.size(); ++i)
        fft(fft_input[i], fft_output[i]);

    fft_input.clear();

    std::freopen("out", "w", stdout);

    for(auto &waves: fft_output) {
        for(auto &[frequency, amplitude, phase]: waves) {
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