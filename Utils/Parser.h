//
// Created by BartKrups on 13.01.2025.
//

#ifndef PARSER_H
#define PARSER_H


#include <vector>
#include <string>

#include "Structures.h"

class Parser {
public:
    static Graph loadGraphFromXML(const std::string& filePath);
};



#endif //PARSER_H
