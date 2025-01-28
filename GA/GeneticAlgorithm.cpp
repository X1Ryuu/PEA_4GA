//
// Created by Bart on 25.01.2025.
//

#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include "GeneticAlgorithm.h"
using namespace std;




/*void GeneticAlgorithm::initializePopulation() {
    int size = costMatrix.getSize();
    uniform_int_distribution<int> dist(0, size - 1);

    for (int i = 0; i < populationSize; ++i) {
        vector<int> path(size);
        iota(path.begin(), path.end(), 0); // Initialize with 0, 1, ..., size-1
        shuffle(path.begin(), path.end(), rng);

        population.push_back({path, calculateFitness(path)});
        *//*int fitness = calculateFitness(path);
        population.push_back({path, fitness});*//*
    }
}*/


void GeneticAlgorithm::filterPopulation() {
    // Posortuj populację według fitness (rosnąco, jeśli minimalizujemy)
    sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
        return a.fitness < b.fitness;
    });

    // Zachowaj najlepsze 50% osobników
    int eliteSize = populationSize / 2;
    vector<Individual> newPopulation(population.begin(), population.begin() + eliteSize);

    // Uzupełnij populację nowymi losowymi osobnikami
    for (int i = eliteSize; i < populationSize; ++i) {
        vector<int> path(costMatrix.getSize());
        iota(path.begin(), path.end(), 0); // Permutacja: 0, 1, ..., size-1
        shuffle(path.begin(), path.end(), rng); // Losowe tasowanie
        newPopulation.push_back({path, calculateFitness(path)});
    }

    // Zastąp starą populację nową
    population = newPopulation;
}


vector<int> GeneticAlgorithm::nearestNeighborHeuristic() {
    int size = costMatrix.getSize();
    vector<int> path;
    vector<bool> visited(size, false);
    uniform_int_distribution<int> gen(0, costMatrix.getSize()-1);
    int currentCity = gen(rng);
    //cout << currentCity << "\n";
   // int currentCity = 0; // Start od pierwszego miasta
    path.push_back(currentCity);
    visited[currentCity] = true;

    for (int i = 1; i < size; ++i) {
        int nextCity = -1;
        int minCost = INT_MAX;

        // Znajdź najbliższe nieodwiedzone miasto
        for (int j = 0; j < size; ++j) {
            if (!visited[j] && costMatrix.getValue(currentCity, j) < minCost) {
                minCost = costMatrix.getValue(currentCity, j);
                nextCity = j;
            }
        }

        path.push_back(nextCity);
        visited[nextCity] = true;
        currentCity = nextCity;
    }

    return path;
}



void GeneticAlgorithm::initializePopulation() {
    int size = costMatrix.getSize();
    uniform_int_distribution<int> dist(0, size - 1);

    // Proporcje losowych i heurystycznych osobników
    int randomIndividuals = populationSize * 0.2;  // 70% losowych
    int heuristicIndividuals = populationSize - randomIndividuals;  // 30% heurystycznych

    // Generowanie losowych osobników
    for (int i = 0; i < randomIndividuals; ++i) {
        vector<int> path(size);
        iota(path.begin(), path.end(), 0); // Permutacja: 0, 1, ..., size-1
        shuffle(path.begin(), path.end(), rng); // Tasowanie
        population.push_back({path, calculateFitness(path)});
    }

    // Generowanie heurystycznych osobników
    for (int i = 0; i < heuristicIndividuals; ++i) {
        vector<int> path = nearestNeighborHeuristic();
        population.push_back({path, calculateFitness(path)});
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




void GeneticAlgorithm::crossOver2Opt(Individual& parent1, Individual& parent2) {
    // Make a copy of the parents' paths
    vector<int> child1 = parent1.path;
    vector<int> child2 = parent2.path;

    // Randomly select two indices for the 2-opt operation
    uniform_int_distribution<int> dist(0, parent1.path.size() - 1);
    int i = dist(rng);
    int j = dist(rng);

    if (i > j) swap(i, j);

    // Apply the 2-opt swap
    reverse(child1.begin() + i, child1.begin() + j + 1);

    // Recalculate fitness for the children
    parent1.path = child1;
    parent1.fitness = calculateFitness(parent1.path);
    parent2.path = child2;
    parent2.fitness = calculateFitness(parent2.path);
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

void GeneticAlgorithm::crossOverOX(Individual& parent1, Individual& parent2) {
    int size = parent1.path.size();
    uniform_int_distribution<int> dist(0, size - 1);

    // Randomly select two crossover points
    int point1 = dist(rng);
    int point2 = dist(rng);

    if (point1 > point2) swap(point1, point2);

    // Initialize child paths
    vector<int> child1(size, -1);
    vector<int> child2(size, -1);

    // Copy the segment from parent1 to child1 and from parent2 to child2
    for (int i = point1; i <= point2; ++i) {
        child1[i] = parent1.path[i];
        child2[i] = parent2.path[i];
    }

    // Fill the remaining positions in child1 and child2 with the remaining elements from parent2 and parent1, respectively
    int idx1 = (point2 + 1) % size;
    int idx2 = (point2 + 1) % size;
    for (int i = 0; i < size; ++i) {
        if (find(child1.begin(), child1.end(), parent2.path[i]) == child1.end()) {
            child1[idx1] = parent2.path[i];
            idx1 = (idx1 + 1) % size;
        }
        if (find(child2.begin(), child2.end(), parent1.path[i]) == child2.end()) {
            child2[idx2] = parent1.path[i];
            idx2 = (idx2 + 1) % size;
        }
    }

    // Assign the children paths to the parents
    parent1.path = child1;
    parent2.path = child2;

    // Recalculate fitness for the children
    parent1.fitness = calculateFitness(parent1.path);
    parent2.fitness = calculateFitness(parent2.path);
}


void GeneticAlgorithm::tournamentSelection(vector<Individual>& selected) {
    int tournamentSize = 3; // Liczba osobników biorących udział w turnieju
    for (int i = 0; i < tournamentSize; ++i) {
        uniform_int_distribution<int> dist(0, population.size() - 1);
        int index = dist(rng);
        selected.push_back(population[index]);
    }

    // Wybór najlepszego osobnika
    sort(selected.begin(), selected.end(), [](const Individual& a, const Individual& b) {
        return a.fitness < b.fitness; // Mniejszy koszt (fitness) oznacza lepszy osobnik
    });
}


void GeneticAlgorithm::evolve() {
    vector<Individual> newPopulation;

    for (size_t i = 0; i < populationSize; i += 2) {
        vector<Individual> selected;
        tournamentSelection(selected);

        Individual parent1 = population[i];
        Individual parent2 = population[i + 1];

        //printf("%lf, max: %lf, %lf, %lf\n", (double)rng(), rng.max(), crossoverRate, (double)rng() / rng.max());
        if ((double)rng() / rng.max() < crossoverRate) {
            if (crossOption==0){
                crossOverOX(parent1, parent2);
            } else if (crossOption==1){
                crossOver2Opt(parent1, parent2);
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
    filterPopulation();
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
    std::ofstream file("../wyniki/results090.csv");
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
