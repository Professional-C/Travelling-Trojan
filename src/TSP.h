#pragma once
#include <string>
#include <vector>
#include <random>
 struct Location
{
	std::string mName;
	double mLatitude;
	double mLongitude;
};

struct Population
{
	std::vector<std::vector<int>> mMembers;
};

double computeHaversine(double lat1, double lat2, double lon1, double lon2);

std::vector<std::pair<int,double>> computeFitness(Population population ,std::vector<Location> locations);

std::vector<std::pair<int,int>> selection(std::vector<std::pair<int,double>> fitnesses, std::mt19937& rand);

Population crossover(std::vector<std::pair<int,int>> pairs, std::vector<std::vector<int>> mMembers, std::mt19937& rand);


