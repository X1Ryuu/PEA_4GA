//
// Created by Bart on 25.01.2025.
//

#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include "GeneticAlgorithm.h"

#include <unordered_map>
#include <unordered_set>
using namespace std;




vector<int> GeneticAlgorithm::nearestNeighborHeuristic() {
    int size = costMatrix.getSize();
    random_device dev;
    mt19937 rng(dev());
    vector<int> path;
    vector<bool> visited(size, false);
    uniform_int_distribution<int> gen(0, costMatrix.getSize()-1);
    int currentCity = gen(rng);

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
    random_device dev;
    mt19937 rng(dev());
    // Proporcje losowych i heurystycznych osobników
    int randomIndividuals = populationSize * 0.7;  // 70% losowych
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









void GeneticAlgorithm::mutateInversion(Individual& individual) {
    uniform_int_distribution<int> dist(0, individual.path.size() - 1);
    random_device dev;
    mt19937 rng(dev());
    int point1 = dist(rng);
    int point2 = dist(rng);

    if (point1 > point2) swap(point1, point2);
    reverse(individual.path.begin() + point1, individual.path.begin() + point2 + 1);

    individual.fitness = calculateFitness(individual.path);
}

void GeneticAlgorithm::mutateSwap(Individual& individual) {
    uniform_int_distribution<int> dist(0, individual.path.size() - 1);
    random_device dev;
    mt19937 rng(dev());
    int point1 = dist(rng);
    int point2 = dist(rng);

    swap(individual.path[point1], individual.path[point2]);
    individual.fitness = calculateFitness(individual.path);
}

void GeneticAlgorithm::crossOverOX(Individual& parent1, Individual& parent2) {
    int size = parent1.path.size();
    uniform_int_distribution<int> dist(0, size - 1);
    random_device dev;
    mt19937 rng(dev());
    // Randomly select two crossover points
    int point1 = dist(rng);
    int point2 = dist(rng);
 //   printf("%d, %d\n", point1, point2);
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
        //    cout << child1[idx1] << ", " << parent2.path[i] <<endl;
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





Individual GeneticAlgorithm::tournamentSelection(int tournamentSize) {
    vector<Individual> tournamentGroup;
    random_device dev;
    mt19937 rng(dev());
    for (int i = 0; i < tournamentSize; ++i) {
        int randomIndex = uniform_int_distribution<int>(0, population.size() - 1)(rng);
        tournamentGroup.push_back(population[randomIndex]);
    }

    // Wybierz najlepszego z grupy
    return *min_element(tournamentGroup.begin(), tournamentGroup.end(), [](const Individual& a, const Individual& b) {
        return a.fitness < b.fitness;
    });
}


void GeneticAlgorithm::evolve() {
    vector<Individual> newPopulation;
    vector<Individual> tempPop;

    sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
        return a.fitness < b.fitness;
    });

    newPopulation.insert(newPopulation.end(), population.begin(), population.begin() + populationSize/4);
    //zastosowanie elitaryzmu dla 25% najlepszych osobników
    //zostaną dodani w niezmienionej postaci do nowej populacji, jednak mogą także wykorzystane do utworzenia nowych potomków




    //for (size_t i = 0; i < populationSize; i += 2) {
    while(newPopulation.size()<populationSize){
     //   cout << populationSize << "\n";
        Individual parent1, parent2;
        parent1 = tournamentSelection(5);
        parent2 = tournamentSelection(5);

        random_device dev;
        mt19937 rng(dev());
        double RNG = (double)rng() / rng.max();

        if (RNG < crossoverRate) {
            crossOverOX(parent1, parent2);
        }

        RNG = (double)rng() / rng.max();
     //   printf("%lf, max: %lf, %lf, %lf\n", (double)rng(), rng.max(), crossoverRate, RNG);
        if (RNG < mutationRate) {
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

pair<vector<int>, int> GeneticAlgorithm::run(string name) {
    initializePopulation();
  //  Best bestBest;
    double elapsedSeconds = 0.0;

    double prev_elapsed=0.0;
    auto startTime = std::chrono::steady_clock::now();

    string filepath = "../wyniki/" + name + ".csv";
    std::ofstream file(filepath, std::ios_base::app);
   // file << optimal << ";" << mutOption << ";" << crossOption << "\n";
    file << "Elapsed time" << ";" << "Relative error" << ";" << "best fitness" << ";" << "optimal" << "\n";

    while (std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count() < timeLimit) {

        evolve();


        Individual best = getBestIndividual();
        double relativeError = std::fabs(bestCost - optimal) / optimal * 100;
      //  cout << relativeError<<", " << elapsedSeconds << ", " << bestBest.fitness   <<", "<< best.fitness <<"\n";
        // Zapisz najlepszy wynik co sekundę
        elapsedSeconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count();
        if (relativeErrorData.empty() || elapsedSeconds - relativeErrorData.back().first >= 1.0) {
            relativeErrorData.emplace_back(elapsedSeconds, relativeError);
        }
        if (bestCost > best.fitness) {
            bestPath = best.path;
            bestCost = best.fitness;
            //bestBest.fitness = best.fitness;
            //bestBest.path = best.path;
        }

        if (elapsedSeconds - prev_elapsed >= 1.0) {
            prev_elapsed = elapsedSeconds;
            file << elapsedSeconds << ";" << relativeError << ";" << bestCost << ";" << optimal <<"\n";
            cout << relativeError<<", " << elapsedSeconds << ", " << bestCost   <<", "<< best.fitness <<", "<<  to_string(mutOption)<<"\n\n";
            //cout << "Elapsed time: " << elapsedSeconds << " s" << endl;
        }
      //  cout << "Best fitness: " << best.fitness << endl;

    }
    file << "\n\na";


    file.close();

    Individual best = getBestIndividual();

    /*cout << "Best fitness: " << bestCost << endl;
    cout << "Path: ";*/

    /*for(int city : bestPath){
        cout << city << " ";
    }
    cout << endl;*/
    return {bestPath, bestCost};
    //return {best.path, best.fitness};
}

