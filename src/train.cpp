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

    std::cerr << "Successfully parsed " << CSV_COUNT << " files." << std::endl;

    init_fft(DATA_POINT_PER_BLOCK);

    std::cerr << "Initialized FFT of size " << DATA_POINT_PER_BLOCK << "." << std::endl;

    std::vector<std::vector<data_point_t>> fft_input;
    for(std::size_t i = 0; i < CSV_COUNT; ++i) {
        for(std::size_t j = 0; j < DATA_BLOCKS; ++j) {
            if(csv[i][j].size() < DATA_POINT_PER_BLOCK) continue;
            fft_input.push_back(csv[i][j]);
        }
    }

    csv.clear();

    std::cerr << "Created FFT Input Array." << std::endl;

    std::vector<std::vector<wave_t>> fft_output(fft_input.size());
    for(std::size_t i = 0; i < fft_input.size(); ++i)
        fft(fft_input[i], fft_output[i]);

    std::cerr << "Ran FFT on " << fft_output.size() << " inputs." << std::endl; 

    fft_input.clear();

    // for(auto &waves: fft_output) {
    //     for(auto &[frequency, amplitude, phase]: waves) {
    //         // std::cout << "F: " << frequency <<
    //         //     "; A: " << amplitude <<
    //         //     "; P: " << phase <<
    //         //     std::endl;
    //         std::cout << frequency << "," << amplitude << "," << phase << std::endl;
    //     }

    //     std::cout << "=======================" << std::endl;
    // }

    std::vector<ld> seed1(K_MEANS_DIMENSIONS, 2), seed2(K_MEANS_DIMENSIONS, 0);
    trained_k_means_algo_t train_output = train_k_means(seed1, seed2, fft_output);

    for(auto &centr1: train_output.centr1)
        std::cout << centr1 << " ";
    std::cout << std::endl;

    for(auto &centr2: train_output.centr2)
        std::cout << centr2 << " ";
    std::cout << std::endl;

    kill_fft();
    return 0;
}