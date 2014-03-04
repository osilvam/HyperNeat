#ifndef USERFUNCTIONS_H
#define USERFUNCTIONS_H

#include "HyperNeat.hpp"
#include <vector>
#include <cmath>
using namespace std;

#define SIGMOID_CONST 4.9


namespace ANN_USM{

	double OutputNodeFunction(double input);
	double Sigmoid(double input);
	void UserTrain(HyperNeat * hyperneat, vector < double * > inputs, vector < double * > outputs);

}
#endif