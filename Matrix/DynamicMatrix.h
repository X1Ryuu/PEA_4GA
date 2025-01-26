#ifndef DYNAMIC_MATRIX_H
#define DYNAMIC_MATRIX_H

#include <iostream>
#include <stdexcept>

#include "../Utils/Structures.h"

class DynamicMatrix {
private:
    int size;
    int** matrix{};    // Wskaźnik do dynamicznej macierzy

    // Prywatna metoda do alokacji pamięci
    void allocateMemory(int size);

    // Prywatna metoda do zwolnienia pamięci
    void freeMemory();


public:
    void generateRandom();
    void generateSymmetricRandom();
    void fromGraph(const Graph &graph);

    // Konstruktor domyślny
    DynamicMatrix();

    // Konstruktor z rozmiarem
    DynamicMatrix(int size);

    // Konstruktor kopiujący
    DynamicMatrix(const DynamicMatrix& other);

    // Destruktor
    ~DynamicMatrix();

    // Operator przypisania
    DynamicMatrix& operator=(const DynamicMatrix& other);

    // Ustawianie wartości w komórce
    void setValue(int row, int col, int value);

    int getSize();
    void setSize(int size);

    // Pobieranie wartości z komórki
    int getValue(int row, int col) const;


    // Wypisywanie macierzy na standardowe wyjście
    void print() const;

    // Zmiana rozmiaru macierzy
    void resize(int size, int newSize);
};


#endif // DYNAMIC_MATRIX_H
