#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <iostream>


double computeHaversine(double lat1, double lat2, double lon1, double lon2){
    double dlon = lon2-lon1;
    double dlat = lat2-lat1;
    double a = (sin(dlat/2.0)*sin(dlat/2.0)) + (cos(lat1) * cos(lat2) * sin(dlon/2.0)*sin(dlon/2.0));
    double c = 2.0 * atan2(sqrt(a), sqrt(1-a));
    double distance = 3961.0 * c;
    return distance;
}

std::vector<std::pair<int,double>> computeFitness(Population population, std::vector<Location> locations) {
    
    std::vector<std::pair<int,double>> fitnesses;
    for(int i = 0; i < population.mMembers.size(); i++){
        // CHANGE THIS TO ADJACENT_DIFFERENCE
        // ISSUE: CANNOT PASS LOCATIONS TO HAVERSINE FUNCTION
        std::vector<double> havDistances;
        for(int j = 0; j < population.mMembers.at(i).size()-1; j++){
            int locIndex1 = population.mMembers.at(i).at(j);
            Location loc1 = locations.at(locIndex1);
            int locIndex2 = population.mMembers.at(i).at(j+1);
            Location loc2 = locations.at(locIndex2);
            havDistances.push_back(computeHaversine(loc1.mLatitude, loc2.mLatitude, loc1.mLongitude, loc2.mLongitude));
        }
        
        // Add in route back to start
        Location begin = locations.at(population.mMembers.at(i).at(0));
        Location end = locations.at(population.mMembers.at(i).at(population.mMembers.at(i).size()-1));
        havDistances.push_back(computeHaversine(begin.mLatitude, end.mLatitude, begin.mLongitude, end.mLongitude));
        double pathDist = std::accumulate(havDistances.begin(), havDistances.end(), 0.0);
        
        std::pair<int,double> fitness(i,pathDist);
        fitnesses.push_back(fitness);
    }
    return fitnesses;
}

bool compareFit(std::pair<int,double> pair1, std::pair<int,double> pair2){
    return pair1.second < pair2.second;
}

std::vector<std::pair<int,int>> selection(std::vector<std::pair<int,double>> fitnesses, std::mt19937& rand){
    
    std::vector<std::pair<int,int>> selections;
    
    //Sort fitnesses by best fit (low to high)
    std::sort(fitnesses.begin(),fitnesses.end(),compareFit);
    int size = fitnesses.size();
    
    // Setup probability vector
    std::vector<double> probs(fitnesses.size());
    std::fill(probs.begin(),probs.end(),1.0/size);
    probs.at(fitnesses.at(0).first) *= 6.0;
    probs.at(fitnesses.at(1).first) *= 6.0;
    for(int i = 2; i <= (fitnesses.size()/2) -1; i++){
        probs.at(fitnesses.at(i).first) *= 3.0;
    }
   
    // Re-normalize probability vector
    double sum = accumulate(probs.begin(),probs.end(), 0.0);
    std::transform(probs.begin(),probs.end(),probs.begin(), [&sum](auto &c){ return c/sum;});
    
   
    // Choose random parents
    for( int i = 0; i < size; i++){
        std::uniform_real_distribution<double> dist(0.0,1.0);
        double p1 = dist(rand);
        int index1 = 0;
        double counter = 0;
        for(int i = 0; i < probs.size(); i++){
            counter += probs.at(i);
            if(counter > p1){
                index1 = i;
                break;
            }
        }
        double p2 = dist(rand);
        counter = 0;
        int index2 = 0;
        for(int i = 0; i < probs.size(); i++){
            counter += probs.at(i);
            if(counter >= p2){
                index2 = i;
                break;
            }
        }
        std::pair<int,int> selectedPair(index1,index2);
        selections.push_back(selectedPair);
    }
    
    return selections;
}

Population crossover(std::vector<std::pair<int,int>> pairs, std::vector<std::vector<int>> mMembers, std::mt19937& rand){
    Population newPop;
    for(int i = 0; i < pairs.size(); i++){
        std::pair<int,int> currPair = pairs.at(i);
        std::uniform_int_distribution<int> dist1(1, mMembers.at(i).size()-2);
        int crossIndex = dist1(rand);
        std::uniform_int_distribution<int> dist2(0,1);
        int goesFirst = dist2(rand);
        
        std::cout << "index: " << crossIndex << " pair " << currPair.first << currPair.second << std::endl;
        std::vector<int> child(mMembers.at(currPair.first).size());
        if(goesFirst == 0){
            std::copy_n(mMembers.at(currPair.first).begin(), crossIndex+1, child.begin());
            std::copy_if(mMembers.at(currPair.second).begin(), mMembers.at(currPair.second).end(), child.begin()+crossIndex+1, [child](int i){return find(child.begin(),child.end(),i) == child.end();});
        }
        else{
            std::copy_n(mMembers.at(currPair.second).begin(), crossIndex+1, child.begin());
            std::copy_if(mMembers.at(currPair.first).begin(), mMembers.at(currPair.first).end(), child.begin()+crossIndex+1, [child](int i){return find(child.begin(),child.end(),i) == child.end();});
        }
//        for(int i = 0; i < child.size(); i++){
//            std::cout << child.at(i) << std:: endl;
//        }
        
        //std::uniform_real_distribution<double>
    }
    return newPop;
}
