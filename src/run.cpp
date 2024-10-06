#include <pre.h>
#include <fft.h>
#include <k_means.h>

#include <vector>

#include <cstdio>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <algorithm>
#include <fstream>

constexpr std::size_t CSV_COUNT = 64;

int main(void) {
    std::cout << CSV_COUNT << std::endl;

    init_fft(DATA_POINT_PER_BLOCK);

    trained_k_means_algo_t train_data;
    std::ifstream TRAIN_STREAM("training-data"); 

    train_data.centr1.resize(K_MEANS_DIMENSIONS);
    for(auto &centr1: train_data.centr1) TRAIN_STREAM >> centr1;
    
    train_data.centr2.resize(K_MEANS_DIMENSIONS);
    for(auto &centr2: train_data.centr2) TRAIN_STREAM >> centr2;
    
    std::cerr << "Parsed Training Data." << std::endl;

    std::vector<std::vector<std::size_t>> quakes(CSV_COUNT, std::vector<std::size_t>(0));

    for(std::size_t i = 0; i < CSV_COUNT; ++i) {
        std::vector<std::vector<data_point_t>> next_block; 
        std::size_t it = parse(next_block);
        std::cerr << "Parsed file " << i << "." << std::endl; 

        std::vector<std::vector<data_point_t>> fft_input;

        std::vector<std::size_t> indices;
        for(std::size_t j = 0; j < DATA_BLOCKS; ++j) {
            if(next_block[j].size() < DATA_POINT_PER_BLOCK) {
                std::cerr << "Missing Data: " << it << " + " << j << std::endl; 

                continue;
            }
            fft_input.push_back(next_block[j]);
            indices.push_back(j);
        }

        std::cerr << "Created FFT Input Array." << std::endl;

        std::vector<std::vector<wave_t>> fft_output(fft_input.size());
        for(std::size_t j = 0; j < fft_input.size(); ++j)
            fft(fft_input[j], fft_output[j]);

        std::cerr << "Ran FFT on " << fft_output.size() << " inputs." << std::endl;

        std::vector<ld> query;
        for(std::size_t j = 0; j < fft_output.size(); ++j) {
            query.clear();

            for(auto &[frequency, amplitude]: fft_output[j])
                query.push_back(amplitude), query.push_back(frequency);

            bool is_quake = k_means_query(train_data, query);
            std::cerr << it << " + " << indices[j] << ": " << (is_quake ? "Quake." : "Noise.") << std::endl;

            if(!is_quake) continue;

            quakes[it].push_back(indices[j]);
        }
    }

    constexpr std::size_t hours3 = 3600 * 3;

    for(auto &qs: quakes) {
        std::cout << qs.size() << " ";
        for(auto &q: qs) std::cout << hours3 * q << " " << hours3 * (q + 1) - 1 << " ";
        std::cout << std::endl;
    }

    kill_fft();
    return 0;
}