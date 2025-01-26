//
// Created by BartKrups on 13.01.2025.
//

#ifndef STRUCTURES_H
#define STRUCTURES_H


#include <vector>


struct Edge {
    int target;
    double cost;
};


struct Graph {
    std::string name;
    std::string source;
    std::string description;
    std::vector<std::vector<Edge>> list;
};


#endif //STRUCTURES_H
