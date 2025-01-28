#include <iostream>
#include <cmath>
#include <iostream>
#include <map>
#include <fstream>
#include <conio.h>
#include "Utils/Parser.h"
#include "Matrix/DynamicMatrix.h"
#include "Utils/Utils.h"
#include "GA/GeneticAlgorithm.h"

using namespace std;
void readConfig()
{
    map<std::string, std::string> config = readConfigFile("config.txt");

    string fileName = config["inputFileName"];
    double coolRate = std::stod(config["coolingRate"]);
    int optimal = std::stoi(config["optimal"]);
    double timeLimit = std::stod(config["timeLimit"]);
    int choice = std::stoi(config["choice"]);

 //   printf("%s\n, %lf, %d, %lf, %d", fileName.c_str(), coolRate, optimal, timeLimit, choice);


    auto graph = Parser::loadGraphFromXML(fileName);
    DynamicMatrix matrix;
    matrix.fromGraph(graph);
  //  GeneticAlgorithm algorithm(matrix, 100, 0.8, 0.01, 0, 120);
    //matrix.print();



}




void test()
{
    // 3 plik
    // 2 krzyżowania
    // 2 mutacje
    // a) 3 wielkości populacji, dla wsp. krzyżowania 0.8, dla wsp. mutacji 0.01, dla czasu 120 sek.
    // b) dla najlepszej populacji, dla wsp. krzyżowania 0.8, dla 3 różnych wsp mutacji: 0.01, 0.05, 0.01
    // c) dla najlepszej populacji, dla wsp. mutacji 0.01, dla 3 różnych wsp krzyżowania: 0.5, 0.7, 0.9
    //3 pliki
    //2 mutacje
    //2 crossover









    string file_path[] = {"../tests/ftv47.xml", "../tests/ftv170.xml", "../tests/rbg403.xml"}; // Ścieżka do pliku XML
    string names[] = {"ftv47", "ftv170", "rbg403"};
    int optimals[] = {1776, 2755, 2465};
    int populacje[] = {100, 200, 300};
    string krzyz[] = {"OX", "2-opt"};
    string mut[] = {"inversion", "swap"};
    double krzyzowania[] = {0.5, 0.7, 0.9};
    double mutacje[] = {0.01, 0.05, 0.1};
    for(int i : optimals){
        cout << "i: " << i << endl;
    }


    for(int numFile=0; numFile<3; numFile++){
        for(int numCross=0; numCross<2; numCross++){
            for(int numMut=0; numMut<2; numMut++)
            {
            }
        }
    }








    for(int i=0; i<3; i++)//3 pliki
    {

        auto graph = Parser::loadGraphFromXML(file_path[i]);
        DynamicMatrix matrix;
        matrix.fromGraph(graph);
        cout << matrix.getSize()<<"\n\n";
        for(int j = 0; j < 3; j++)
        {
            for(int k = 0; k < 2; k++){
                for(int l = 0; l < 2; l++){
                    cout << "Populacja: " << populacje[j] << ", Krzyzowanie: " << krzyz[k] << ", Mutacja: " << mut[l] << "\n";
                    GeneticAlgorithm ga(matrix, populacje[j], 0.8, 0.01, k, l, /*(i+3)*60*/(i+1)*120, optimals[i]);
                    // std::cout<<"File: " << i << " iter: "<< k << ", method: " << 0 <<std::endl;
                    auto [bestPath, bestDistance] = ga.run(names[j]);
                    std::cout << "Najlepsza znaleziona trasa:\n";
                    for (int city : bestPath) {
                        std::cout << city << " -> ";
                    }
                    std::cout << bestPath[0] << std::endl;
                    std::cout << "Calkowita dlugosc trasy: " << bestDistance << std::endl;
                    std::cout << "Blad wzgledny [%]: " << std::fabs(bestDistance - optimals[i]) / optimals[i] * 100 << std::endl;
                    cout << "\n\n";
                }
            }



        }
    }


}




int main(){
    readConfig();
    std::cout << "Hello, World!" << std::endl;
    random_device dev;
    mt19937 rng(dev());

    std::uniform_int_distribution<mt19937::result_type> dis(0, 6 - 1);
    printf("%d, %d", dis(rng), dis(rng));
    test();
    return 0;
}



