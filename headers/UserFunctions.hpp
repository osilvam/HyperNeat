#ifndef USERFUNCTIONS_H
#define USERFUNCTIONS_H

#include "HyperNeat.hpp"
#include <vector>
using namespace std;
using namespace ANN_USM;

// 	USER DEFINITIONS

#define POPULATION_MAX 2
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

#define FUNCTION_NUM 6
#define GENERATIONS 2

// --------- SIGMOID FUNCTION DEFINE --------- //
#define SIGMOID_CONST 4.9 
// ---------------- FOR OCTAVE --------------- //
#define OCTAVE_SIGMOID_STATEMENT "function [y] = SIGMOID(x)"
#define OCTAVE_SIGMOID_CONST_LETTER "K"
#define OCTAVE_SIGMOID_CONST SIGMOID_CONST 
#define OCTAVE_SIGMOID_FUNC "y = (1/(1+exp(-K*x)) - 0.5)*2"
// =========================================== //

#define SIGMOID(X) (double)((1 / (1 + exp(-SIGMOID_CONST*X)))-0.5)*2.0
#define SIGMOID_STRING "#define SIGMOID(X) (double)((1 / (1 + exp(-SIGMOID_CONST*X)))-0.5)*2.0"

#define NODE_FUNCTION(X) SIGMOID(X)
#define NODE_FUNCTION_TEXT "SIGMOID"
#define HYPERNEAT_TEST "QUADRATOT"

namespace ANN_USM{

	void GetNodeFunction(string plataform);
	double OutputNodeFunction(double input);
	double Sigmoid(double input);
	//double USER_CUSTOM(double input);
	void UserTrain(HyperNeat * hyperneat, vector < double * > inputs, vector < double * > outputs);

}
#endif