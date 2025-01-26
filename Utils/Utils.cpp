//
// Created by BartKrups on 12.01.2025.
//

#include "Utils.h"

#include <fstream>
#include <random>
using namespace std;

map<std::string, std::string> readConfigFile(const std::string& filename) {
    std::map<std::string, std::string> config;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Could not open config file: " << filename << std::endl;
        return config;
    }

    while (std::getline(file, line)) {
        size_t delimiterPos = line.find('=');
        std::string key = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);

        config[key] = value;
    }

    file.close();
    return config;
}


void printMatrix(const std::vector<std::vector<int>> &matrix) {
    for (const auto &row : matrix) {
        for (const auto &element : row) {
            std::cout << element << "\t"; // Elementy wiersza oddzielone tabulatorem
        }
        std::cout << std::endl; // Nowa linia po każdym wierszu
    }
}