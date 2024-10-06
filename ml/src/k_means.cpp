#include <k_means.h>
#include <iostream>

std::vector<ld> centr1, centr2;

ld squared_euclidean_distance(std::vector<ld> &pointA, std::vector<ld> &pointB) {
    ld res = 0;
    for (std::size_t i = 0; i < pointA.size(); i++) {
        res += (pointA[i] - pointB[i]) * (pointA[i] - pointB[i]);
    }
    return res;
}

trained_k_means_algo_t train_k_means(std::vector<ld> &seed1,
   std::vector<ld> &seed2, std::vector<std::vector<wave_t>> &data) {
    centr1 = seed1;
    centr2 = seed2;

    std::cerr << "seed1, centr1 size: " << seed1.size() << ", " << centr1.size() << std::endl;

    std::vector<ld> distances1(data.size());
    std::vector<ld> distances2(data.size());

    std::vector<std::vector<ld>> dataPoints(data.size());
    for (std::size_t i = 0; i < data.size(); i++) {
        dataPoints[i].resize(K_MEANS_DIMENSIONS);
        for (std::size_t j = 0; j < data[i].size(); j++) {
            dataPoints[i][j * 2]     = data[i][j].amplitude;
            dataPoints[i][j * 2 + 1] = data[i][j].frequency;
        }
    }

    std::vector<std::size_t> prevAssigned1;
    std::vector<std::size_t> prevAssigned2;

    while (true) {
        std::cerr << prevAssigned1.size() << " " << prevAssigned2.size() << std::endl;

        for(auto &c1: centr1)
            std::cerr << c1 << " ";
        std::cerr << std::endl;

        for(auto &c2: centr2)
            std::cerr << c2 << " ";
        std::cerr << std::endl;
        
        std::cerr << "===========================================" << std::endl;

        for (std::size_t i = 0; i < data.size(); i++) {
            distances1[i] = squared_euclidean_distance(dataPoints[i], centr1);
            distances2[i] = squared_euclidean_distance(dataPoints[i], centr2);
        }
        std::vector<std::size_t> assigned1;
        std::vector<std::size_t> assigned2;
        for (std::size_t i = 0; i < data.size(); i++) {
            if (distances1[i] < distances2[i]) {
                assigned1.push_back(i);
            } else {
                assigned2.push_back(i);
            }
        }
        if (assigned1 == prevAssigned1 && assigned2 == prevAssigned2)
            break;

        std::vector<ld> newCentr1(centr1.size());
        std::vector<ld> newCentr2(centr2.size());
        for (auto &i: assigned1) {
            for (std::size_t j = 0; j < dataPoints[i].size(); j++) {
                newCentr1[j] += dataPoints[i][j];
            }
        }
        for (std::size_t i = 0; i < newCentr1.size(); i++) {
            newCentr1[i] /= assigned1.size();
        }
        for (auto &i: assigned2) {
            for (std::size_t j = 0; j < dataPoints[i].size(); j++) {
                newCentr2[j] += dataPoints[i][j];
            }
        }
        for (std::size_t i = 0; i < newCentr2.size(); i++) {
            newCentr2[i] /= assigned2.size();
        }
        centr1 = newCentr1;
        centr2 = newCentr2;

        prevAssigned1 = assigned1;
        prevAssigned2 = assigned2;
    }
    return {centr1, centr2};
}

bool k_means_query(trained_k_means_algo_t &train_data, std::vector<ld> &queryPoint) {
    if (squared_euclidean_distance(queryPoint, train_data.centr1) <
        squared_euclidean_distance(queryPoint, train_data.centr2))
        return true;
    return false;
}