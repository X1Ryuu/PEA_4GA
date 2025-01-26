#include "DynamicMatrix.h"

#include <random>

#include "../Utils/pugixml/pugixml.hpp"
using namespace std;

// Prywatna metoda do alokacji pamięci
void DynamicMatrix::allocateMemory(int size) {
    matrix = new int*[size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new int[size]{0}; // Wypełnienie zerami
    }
}

// Prywatna metoda do zwolnienia pamięci
void DynamicMatrix::freeMemory() {
    if (matrix != nullptr) {
        for (int i = 0; i < size; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
        matrix = nullptr;
    }
}

// Konstruktor domyślny
DynamicMatrix::DynamicMatrix() : size(0), matrix(nullptr) {}

// Konstruktor z rozmiarem
DynamicMatrix::DynamicMatrix(int size) :size(size) {
    allocateMemory(size);
}

// Konstruktor kopiujący
DynamicMatrix::DynamicMatrix(const DynamicMatrix& other)
    : size(other.size){
    allocateMemory(size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = other.matrix[i][j];
        }
    }
}

// Destruktor
DynamicMatrix::~DynamicMatrix() {
    freeMemory();
}

// Operator przypisania
DynamicMatrix& DynamicMatrix::operator=(const DynamicMatrix& other) {
    if (this != &other) {
        freeMemory();
        size = other.size;
        allocateMemory(size);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                matrix[i][j] = other.matrix[i][j];
            }
        }
    }
    return *this;
}

// Ustawianie wartości w komórce
void DynamicMatrix::setValue(int row, int col, int value) {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        cout << "Row: " << row << " Col: " << col << " Value: " << value << endl;
        throw std::out_of_range("Index out of bounds");
    }
    matrix[row][col] = value;
}

// Pobieranie wartości z komórki
int DynamicMatrix::getValue(int row, int col) const {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        cout << "Row: " << row << " Col: " << col <<  endl;
        throw std::out_of_range("Index out of bounds");
    }
    return matrix[row][col];
}



int DynamicMatrix::getSize()
{
    return size;
}

void DynamicMatrix::setSize(int size)
{
    this->size = size;
}


// Wypisywanie macierzy na standardowe wyjście
void DynamicMatrix::print() const {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

// Zmiana rozmiaru macierzy
void DynamicMatrix::resize(int size, int newSize) {
    DynamicMatrix temp(newSize);
    int minRows = std::min(size, newSize);
    int minCols = std::min(size, newSize);

    for (int i = 0; i < minRows; ++i) {
        for (int j = 0; j < minCols; ++j) {
            temp.matrix[i][j] = matrix[i][j];
        }
    }

    *this = temp;
}





void DynamicMatrix::generateSymmetricRandom()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,100);
    for(int i = 0; i < getSize(); i++){
        for(int j = 0; j < getSize(); j++){
            if(i == j)
                matrix[i][j] = -1;
            else if(i > j)
                matrix[i][j] = matrix[j][i];
            else
                matrix[i][j] = dist6(rng);
        }
    }
}


void DynamicMatrix::generateRandom()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,100);
    for(int i = 0; i < getSize(); i++){
        for(int j = 0; j < getSize(); j++){
            if(i == j)
                matrix[i][j] = -1;
            else
                matrix[i][j] = dist6(rng);
        }
    }
}

void DynamicMatrix::fromGraph(const Graph &graph) {
    // Set size from graph

    this->size = graph.list.size();
    // Allocate memory for the matrix
    if (this->size > 0) {
        this->matrix = new int*[this->size];
        for (int i = 0; i < this->size; i++) {
            this->matrix[i] = new int[this->size]();
        }
    } else {
        std::cerr << "Error Matrix.cpp: Graph is empty, cannot create a matrix." << std::endl;
        return;
    }

    // Fill the matrix with costs from the graph
    for (int i = 0; i < graph.list.size(); ++i) {
        for (const auto& edge : graph.list[i]) {
            // Convert double cost to int (since matrix stores ints)
            this->matrix[i][edge.target] = static_cast<int>(edge.cost);
        }
    }

}

/*

void DynamicMatrix::print() const {
    if (matrix == nullptr) {
        std::cout << "Macierz jest pusta." << std::endl;
        return;
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cout << matrix[i][j] << "\t"; // Elementy oddzielone tabulatorem
        }
        std::cout << std::endl; // Nowa linia po każdym wierszu
    }
    std::cout << std::endl; // Dodatkowa linia dla czytelności
}*/
