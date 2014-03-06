#ifndef USERFUNCTIONS_H
#define USERFUNCTIONS_H

#include "HyperNeat.hpp"
#include <vector>
using namespace std;
using namespace ANN_USM;

// 	USER DEFINITIONS

#define POPULATION_MAX 150
#define DISTANCE_CONST_1 1.0
#define DISTANCE_CONST_2 0.4
#define DISTANCE_CONST_3 1.0
#define DISTANCE_CONST_4 1.0
#define DISTANCE_THRESHOLD 3.0

#define PERCENT_MUTATION_CONNECTION 0.25
#define PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER 25
#define PROBABILITY_INTERSPACIES_MATING 0.01
#define SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE 0.07
#define SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION 0.05
#define LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE 0.01
#define LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION 0.03
#define PROBABILITY_CONNECTION_WEIGHT_MUTATING 80
#define LARGE_POPULATION_DISCRIMINATOR (POPULATION_MAX/10.0)

#define FUNCTION_NUM 7
#define GENERATIONS 30

// --------- SIGMOID FUNCTION DEFINE --------- //
#define SIGMOID_FUNC "(1/(1+exp(-SIGMOID_CONST*X))"
#define SIGMOID_CONST 4.9 
// =========================================== //

#define NODE_FUNCTION "SIGMOID"

namespace ANN_USM{

	string GetNodeFunctionInfo();
	double OutputNodeFunction(double input);
	double Sigmoid(double input);
	//double USER_CUSTOM(double input);
	void UserTrain(HyperNeat * hyperneat, vector < double * > inputs, vector < double * > outputs);

}
#endif