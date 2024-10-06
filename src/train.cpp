#include <pre.h>
#include <fft.h>
#include <k_means.h>

#include <vector>

#include <cstdio>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <algorithm>

constexpr std::size_t CSV_COUNT = 76;

int main(void) {
    // std::vector<std::vector<std::vector<data_point_t>>> csv(CSV_COUNT);
    // for(auto &csv_block: csv) parse(csv_block);

    // std::cerr << "Successfully parsed " << CSV_COUNT << " files." << std::endl;

    init_fft(DATA_POINT_PER_BLOCK);

    std::cerr << "Initialized FFT of size " << DATA_POINT_PER_BLOCK << "." << std::endl;

    std::vector<std::vector<data_point_t>> fft_input;
    // for(std::size_t i = 0; i < CSV_COUNT; ++i) {
    //     for(std::size_t j = 0; j < DATA_BLOCKS; ++j) {
    //         if(csv[i][j].size() < DATA_POINT_PER_BLOCK) continue;
    //         fft_input.push_back(csv[i][j]);
    //     }
    // }

    // csv.clear();

    std::size_t N; std::cin >> N;
    fft_input.resize(N);

    for(std::size_t i = 0; i < N; ++i) {
        fft_input[i].resize(DATA_POINT_PER_BLOCK);

        for(std::size_t j = 0; j < DATA_POINT_PER_BLOCK; ++j) {
            auto &[time, velocity] = fft_input[i][j];

            std::cin >> time >> velocity;
        }
    }

    std::cerr << "Created FFT Input Array." << std::endl;

    std::vector<std::vector<wave_t>> fft_output(fft_input.size());
    for(std::size_t i = 0; i < fft_input.size(); ++i)
        fft(fft_input[i], fft_output[i]);

    std::cerr << "Ran FFT on " << fft_output.size() << " inputs." << std::endl; 

    fft_input.clear();
    
    auto get_key = [&](const std::vector<wave_t> &v) {
        return (std::size_t)(v.begin() - fft_output[0].begin());
    };

    std::unordered_map<std::size_t, ld> avg_amplitude;
    for(auto &waves: fft_output) {
        ld avg = 0;
        for(auto &[frequency, amplitude]: waves)
            avg += amplitude;

        avg /= (ld)waves.size();
        avg_amplitude[get_key(waves)] = avg;
    }

    std::sort(fft_output.begin(), fft_output.end(),
       [&](const std::vector<wave_t> &a, const std::vector<wave_t> &b) {
        return avg_amplitude[get_key(a)] > avg_amplitude[get_key(b)];
    });

    std::size_t index_max = fft_output.size() *  1 / 8;
    std::size_t index_min = fft_output.size() - 1;

    std::vector<ld> seed1;
    for(auto &[frequency, amplitude]: fft_output[index_max])
        seed1.push_back(amplitude), seed1.push_back(frequency);

    std::vector<ld> seed2;
    for(auto &[frequency, amplitude]: fft_output[index_min])
        seed2.push_back(amplitude), seed2.push_back(frequency);

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