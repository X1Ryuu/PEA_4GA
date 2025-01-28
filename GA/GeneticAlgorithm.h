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
    int crossOption;
    int mutOption;
    vector<pair<double, double>> relativeErrorData;
    double lowestError;
    int bestCost;
    vector<int> bestPath;
    double timeLimit;
    int optimal;
    random_device dev;
    mt19937 rng;
    //uniform_int_distribution<double> gen(0, 1);
    void initializePopulation();
    void filterPopulation();
    void crossOver2Opt(Individual& parent1, Individual& parent2);
    int calculateFitness(const vector<int>& path);
    Individual randomSelection();
    Individual rouletteWheelSelection();
    Individual tournamentSelection(int tournamentSize);
    void rouletteWheelSelection(Individual& selected);
    vector<int> nearestNeighborHeuristic();
    void crossoverOnePoint(Individual& parent1, Individual& parent2);
    void crossOverOX(Individual& parent1, Individual& parent2);
    void crossOverPMX(Individual& parent1, Individual& parent2);
    void crossoverTwoPoint(Individual& parent1, Individual& parent2);
    void mutateInversion(Individual& individual);
    void mutateSwap(Individual& individual);
    void evolve();
    Individual getBestIndividual();
    void tournamentSelection(vector<Individual>& selected);
public:
    GeneticAlgorithm(const DynamicMatrix& matrix, int popSize, double crossRate, double mutRate, int crossChoice, int mutChoice, double timeLimit, int optimal)
            : costMatrix(matrix), populationSize(popSize), crossoverRate(crossRate), mutationRate(mutRate), rng(dev()), crossOption(crossChoice), mutOption(mutChoice), timeLimit(timeLimit), optimal(optimal) {
        bestCost = INT_MAX;
    }

    pair<vector<int>, int> run(string name);
};


#endif //PEA4_GENETICALGORITHM_H
