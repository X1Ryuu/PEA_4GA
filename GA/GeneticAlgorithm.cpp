//
// Created by Bart on 25.01.2025.
//

#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include "GeneticAlgorithm.h"
using namespace std;




void GeneticAlgorithm::initializePopulation() {
    int size = costMatrix.getSize();
    uniform_int_distribution<int> dist(0, size - 1);

    for (int i = 0; i < populationSize; ++i) {
        vector<int> path(size);
        iota(path.begin(), path.end(), 0); // Initialize with 0, 1, ..., size-1
        shuffle(path.begin(), path.end(), rng);

        population.push_back({path, calculateFitness(path)});
        /*int fitness = calculateFitness(path);
        population.push_back({path, fitness});*/
    }
}

int GeneticAlgorithm::calculateFitness(const vector<int>& path) {
    int totalCost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        totalCost += costMatrix.getValue(path[i], path[i + 1]);
    }
    totalCost += costMatrix.getValue(path.back(), path.front()); // Return to start
    return totalCost;
}

void GeneticAlgorithm::crossOverOX(Individual& parent1, Individual& parent2)
{



}


void GeneticAlgorithm::crossOverPMX(Individual& parent1, Individual& parent2)
{
    random_device dev;
    mt19937 rng(dev());

    std::uniform_int_distribution<mt19937::result_type> dis(0, costMatrix.getSize() - 1);
}



void GeneticAlgorithm::crossoverOnePoint(Individual& parent1, Individual& parent2) {
    uniform_int_distribution<int> dist(1, parent1.path.size() - 2);
    int point = dist(rng);

    vector<int> child1 = parent1.path;
    vector<int> child2 = parent2.path;

    for (int i = 0; i < point; ++i) {
        swap(child1[i], child2[i]);
    }

    parent1.path = child1;
    parent2.path = child2;

    parent1.fitness = calculateFitness(parent1.path);
    parent2.fitness = calculateFitness(parent2.path);
}

void GeneticAlgorithm::crossoverTwoPoint(Individual& parent1, Individual& parent2) {
    uniform_int_distribution<int> dist(1, parent1.path.size() - 2);
    int point1 = dist(rng);
    int point2 = dist(rng);

    if (point1 > point2) swap(point1, point2);

    vector<int> child1 = parent1.path;
    vector<int> child2 = parent2.path;

    for (int i = point1; i <= point2; ++i) {
        swap(child1[i], child2[i]);
    }

    parent1.path = child1;
    parent2.path = child2;

    parent1.fitness = calculateFitness(parent1.path);
    parent2.fitness = calculateFitness(parent2.path);
}

void GeneticAlgorithm::mutateInversion(Individual& individual) {
    uniform_int_distribution<int> dist(0, individual.path.size() - 1);
    int point1 = dist(rng);
    int point2 = dist(rng);

    if (point1 > point2) swap(point1, point2);
    reverse(individual.path.begin() + point1, individual.path.begin() + point2 + 1);

    individual.fitness = calculateFitness(individual.path);
}

void GeneticAlgorithm::mutateSwap(Individual& individual) {
    uniform_int_distribution<int> dist(0, individual.path.size() - 1);
    int point1 = dist(rng);
    int point2 = dist(rng);

    swap(individual.path[point1], individual.path[point2]);
    individual.fitness = calculateFitness(individual.path);
}

void GeneticAlgorithm::evolve() {
    vector<Individual> newPopulation;

    for (size_t i = 0; i < populationSize; i += 2) {
        Individual parent1 = population[i];
        Individual parent2 = population[i + 1];


        if ((double)rng() / rng.max() < crossoverRate) {
            if (crossOption==0){
                crossoverOnePoint(parent1, parent2);
            } else if (crossOption==1){
                //crossoverTwoPoint(parent1, parent2);
                //crossoverOrder
            }
        }

/*            if ((double)rng() / rng.max() < mutationRate) {
            mutateInversion(parent1);
            mutateSwap(parent2);
        }*/

        if ((double)rng() / rng.max() < mutationRate) {
            if(mutOption==0){
                mutateInversion(parent1);
            } else if(mutOption==1){
                mutateSwap(parent2);
            }
        }
        newPopulation.push_back(parent1);
        newPopulation.push_back(parent2);
    }

    population = newPopulation;
}

Individual GeneticAlgorithm::getBestIndividual() {
    return *min_element(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
        return a.fitness < b.fitness;
    });
}

pair<vector<int>, int> GeneticAlgorithm::run() {
    initializePopulation();

    double elapsedSeconds = 0.0;
    auto startTime = std::chrono::steady_clock::now();
    std::ofstream file("results090.csv");
    file << optimal << ";" << mutOption << ";" << crossOption << "\n";
    file << "Elapsed time" << ";" << "Relative error" << ";" << "best fitness" << ";" << "optimal" << "\n";
    while (std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count() < timeLimit) {
        evolve();


        Individual best = getBestIndividual();
        double relativeError = std::fabs(best.fitness - optimal) / optimal;

        // Zapisz najlepszy wynik co sekundę
        elapsedSeconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count();
        if (relativeErrorData.empty() || elapsedSeconds - relativeErrorData.back().first >= 1.0) {
            relativeErrorData.emplace_back(elapsedSeconds, relativeError);
        }
        file << elapsedSeconds << ";" << relativeError << ";" << best.fitness << ";" << optimal <<"\n";
    }
    file << "\n\n";
    file.close();

/*        for (int gen = 0; gen < generations; ++gen) {
        evolve();
    }*/

    Individual best = getBestIndividual();
    cout << "Best fitness: " << best.fitness << endl;
    cout << "Path: ";
    for (int city : best.path) {
        cout << city << " ";
    }
    cout << endl;
    return {best.path, best.fitness};
}


/*
void save(int it, int vertices, int bestCost, double bestTime, double totalTime, double error) {
    std::ofstream file("results090.csv", std::ios::app);
    if (file.is_open()) {
        file << it << ";"<<vertices << ";" << bestCost << ";" << initialCost <<";" << bestTime << ";" << totalTime << ";" <<error << "\n";
        file.close();
    }
}*/
