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

struct Best{
    vector<int> path;
    int fitness = INT_MAX;
};


class GeneticAlgorithm {
private:
    DynamicMatrix costMatrix;
    vector<Individual> population;
    int populationSize;
    double crossoverRate;
    double mutationRate;
    int mutOption;
    vector<pair<double, double>> relativeErrorData;
    int bestCost;
    vector<int> bestPath;
    double timeLimit;
    int optimal;

    //uniform_int_distribution<double> gen(0, 1);
    void initializePopulation();

    int calculateFitness(const vector<int>& path);

    Individual tournamentSelection(int tournamentSize);

    vector<int> nearestNeighborHeuristic();

    void crossOverOX(Individual& parent1, Individual& parent2);

    void mutateInversion(Individual& individual);
    void mutateSwap(Individual& individual);

    void evolve();
    Individual getBestIndividual();

public:
    GeneticAlgorithm(const DynamicMatrix& matrix, int popSize, double crossRate, double mutRate, int mutChoice, double timeLimit, int optimal)
            : costMatrix(matrix), populationSize(popSize), crossoverRate(crossRate), mutationRate(mutRate), mutOption(mutChoice), timeLimit(timeLimit), optimal(optimal) {
        bestCost = INT_MAX;
    }
 //   void crossOverOX(Individual& parent1, Individual& parent2);
    GeneticAlgorithm(){bestCost = INT_MAX;};
   // void crossOverPMX(Individual& parent1, Individual& parent2);
    pair<vector<int>, int> run(string name);
};


#endif //PEA4_GENETICALGORITHM_H
