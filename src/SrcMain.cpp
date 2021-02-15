#include "SrcMain.h"
#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <sstream>


std::vector<Location> readLocations(std::string input){
    std::ifstream inputFile(input);
    std::string line;
    std::vector<Location> locations;
    while(std::getline(inputFile,line)){
        std::istringstream tokenizer(line);
        Location loc;
        std::string lat, lon;
        std::getline(tokenizer, loc.mName, ',');
        std::getline(tokenizer, lat, ',');
        std::getline(tokenizer, lon, ',');
        loc.mLatitude = std::stod(lat)*0.0174533;
        loc.mLongitude = std::stod(lon)*0.0174533;
        locations.push_back(loc);
    }
    return locations;
}

Population generatePopulation(int numLocations, int popSize, std::mt19937& rand)
{
    Population population;
    for(int i = 0; i < popSize; i++){
        std::vector<int> locOrder(numLocations);
        std::iota(std::begin(locOrder),std::end(locOrder),0);
        std::uniform_int_distribution<> myDist(0,locOrder.size());
        std::shuffle(std::begin(locOrder)+1, std::end(locOrder), rand);
        population.mMembers.push_back(locOrder);
    }
    return population;
}

void ProcessCommandArgs(int argc, const char* argv[])
{
	// Read in command line args
    std::string input = argv[1];
    int pop = atoi(argv[2]);
    int generations = atoi(argv[3]);
    int mutation = atoi(argv[4]);
    long seed = atol(argv[5]);
    
    // Random number generator constructor
    std::mt19937 randGen(seed);
    
    // Read in locations from input file
    std::vector<Location> locations = readLocations(input);
    
    Population population = generatePopulation(locations.size(), pop, randGen);
    
    std::ofstream output("log.txt");
    output << "INITIAL POPULATION:" << std::endl;
    for(auto it = std::begin(population.mMembers); it != std::end(population.mMembers); it++){
        for(auto it2 = std::begin(*it); it2 != std::end(*it); it2++){
            output << *it2;
            if(it2 != std::end(*it)-1){
                output << ",";
            }
        }
        output << std::endl;
    }
  
    std::vector<std::pair<int,double>> fitness = computeFitness(population, locations);
    
    output << "FITNESS:" << std::endl;
    for(int i = 0; i < fitness.size(); i++){
        output << fitness.at(i).first << ":" << fitness.at(i).second << std::endl;
    }
    
    std::vector<std::pair<int,int>> selectionPairs = selection(fitness, randGen);
    output << "SELECTED PAIRS:" << std::endl;
    for(int i = 0; i < selectionPairs.size(); i++){
        output << "(" << selectionPairs.at(i).first << "," << selectionPairs.at(i).second << ")" << std::endl;
    }
    
    crossover(selectionPairs, population.mMembers, randGen);
    
    output.close();
    
}
