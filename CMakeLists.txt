cmake_minimum_required(VERSION 3.28)
project(PEA4)

set(CMAKE_CXX_STANDARD 17)

add_executable(PEA4 main.cpp
        GA/GeneticAlgorithm.cpp
        GA/GeneticAlgorithm.h
        Utils/Utils.cpp
        Utils/Utils.h
        Matrix/DynamicMatrix.cpp
        Matrix/DynamicMatrix.h
        Utils/Parser.cpp
        Utils/Parser.h
        Utils/pugixml/pugixml.hpp
        Utils/pugixml/pugiconfig.hpp
        Utils/pugixml/pugixml.cpp
)
