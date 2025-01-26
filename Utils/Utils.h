//
// Created by BartKrups on 12.01.2025.
//



#ifndef UTILS_H
#define UTILS_H
#include <map>

#include "../Matrix/DynamicMatrix.h"


std::map<std::string, std::string> readConfigFile(const std::string& filename);
void printMatrix(const std::vector<std::vector<int>> &matrix);

#endif //UTILS_H
