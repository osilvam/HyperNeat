#ifndef USERFUNCTIONS_H
#define USERFUNCTIONS_H

#include "HyperNeat.hpp"
#include <vector>
using namespace std;
using namespace ANN_USM;

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