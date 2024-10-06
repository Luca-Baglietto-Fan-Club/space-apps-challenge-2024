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

constexpr std::size_t CSV_COUNT = 76;

int main(void) {
    std::vector<std::vector<std::vector<data_point_t>>> csv(CSV_COUNT);
    for(auto &csv_block: csv) parse(csv_block);

    std::cerr << "Successfully parsed " << CSV_COUNT << " files." << std::endl;

    init_fft(DATA_POINT_PER_BLOCK);

    std::cerr << "Initialized FFT of size " << DATA_POINT_PER_BLOCK << "." << std::endl;

    std::vector<std::size_t> indexes;
    std::vector<std::vector<data_point_t>> fft_input;
    for(std::size_t i = 0; i < CSV_COUNT; ++i) {
        for(std::size_t j = 0; j < DATA_BLOCKS; ++j) {
            if(csv[i][j].size() < DATA_POINT_PER_BLOCK) continue;
            fft_input.push_back(csv[i][j]);
            indexes.push_back(i * DATA_BLOCKS + j);
        }
    }

    csv.clear();

    // std::size_t N; std::cin >> N;
    // fft_input.resize(N);

    // for(std::size_t i = 0; i < N; ++i) {
    //     fft_input[i].resize(DATA_POINT_PER_BLOCK);

    //     for(std::size_t j = 0; j < DATA_POINT_PER_BLOCK; ++j) {
    //         auto &[time, velocity] = fft_input[i][j];

    //         std::cin >> time >> velocity;
    //     }
    // }

    std::ifstream quake_file("quake");
    std::ifstream noise_file("noise");

    std::vector<std::size_t> quake_indexes;
    std::vector<std::size_t> noise_indexes;

    std::size_t N_quake; quake_file >> N_quake;
    for (std::size_t i = 0; i < N_quake; i++) {
        std::size_t n_quake; quake_file >> n_quake;
        for (std::size_t j = 0; j < n_quake; j++) {
            std::size_t value; quake_file >> value;
            quake_indexes.push_back(DATA_BLOCKS * i + value);
        }
    }

    std::size_t N_noise; noise_file >> N_noise;
    for (std::size_t i = 0; i < N_noise; i++) {
        std::size_t n_noise; noise_file >> n_noise;
        for (std::size_t j = 0; j < n_noise; j++) {
            std::size_t value; noise_file >> value;
            noise_indexes.push_back(DATA_BLOCKS * i + value);
        }
    }

    std::cerr << "Created FFT Input Array." << std::endl;

    std::vector<std::vector<wave_t>> quakes;
    std::vector<std::vector<wave_t>> noise;

    std::vector<std::vector<wave_t>> fft_output(fft_input.size());
    for(std::size_t i = 0; i < fft_input.size(); ++i) {
        fft(fft_input[i], fft_output[i]);
        if (std::find(quake_indexes.begin(), quake_indexes.end(), indexes[i]) != quake_indexes.end()) {
            quakes.push_back(fft_output[i]);
        } else if (std::find(noise_indexes.begin(), noise_indexes.end(), indexes[i]) != noise_indexes.end()) {
            noise.push_back(fft_output[i]);
        }
    }

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

    std::vector<ld> seed1(K_MEANS_DIMENSIONS);
    for (auto &quake: quakes) {
        for (std::size_t j = 0; j < FFT_OUT_APPROX; j++) {
            seed1[j * 2]     += quake[j].amplitude;
            seed1[j * 2 + 1] += quake[j].frequency;
        }
    }

    for (auto &x: seed1) {
        x /= quakes.size();
    }

    std::vector<ld> seed2(K_MEANS_DIMENSIONS);
    for (auto &noi: noise) {
        for (std::size_t j = 0; j < FFT_OUT_APPROX; j++) {
            seed2[j * 2]     += noi[j].amplitude;
            seed2[j * 2 + 1] += noi[j].frequency;
        }
    }
    for (auto &x: seed2) {
        x /= noise.size();
    }

    std::cerr << "seed1, seed2 size: " << seed1.size() << ", " << seed2.size() << std::endl;

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