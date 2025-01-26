//
// Created by Bart on 25.01.2025.
//

#ifndef PEA4_GENETICALGORITHM_H
#define PEA4_GENETICALGORITHM_H

#include <random>
#include "../Matrix/DynamicMatrix.h"

using namespace std;

struct Individual {
    vector<int> path;
    int fitness;
};


class GeneticAlgorithm {
private:
    DynamicMatrix costMatrix;
    vector<Individual> population;
    int populationSize;
    double crossoverRate;
    double mutationRate;
    int crossOption;
    int mutOption;
    vector<pair<double, double>> relativeErrorData;
    double timeLimit;
    int optimal;
    random_device rd;
    mt19937 rng;
    void initializePopulation();

    int calculateFitness(const vector<int>& path);

    void crossoverOnePoint(Individual& parent1, Individual& parent2);

    void crossoverTwoPoint(Individual& parent1, Individual& parent2);
    void mutateInversion(Individual& individual);
    void mutateSwap(Individual& individual);
    void evolve();
    Individual getBestIndividual();
public:
    GeneticAlgorithm(const DynamicMatrix& matrix, int popSize, double crossRate, double mutRate, int crossChoice, int mutChoice, double timeLimit, int optimal)
            : costMatrix(matrix), populationSize(popSize), crossoverRate(crossRate), mutationRate(mutRate), rng(rd()), crossOption(crossChoice), mutOption(mutChoice), timeLimit(timeLimit), optimal(optimal) {}

    pair<vector<int>, int> run();
};


#endif //PEA4_GENETICALGORITHM_H
