#ifndef USERFUNCTIONS_H
#define USERFUNCTIONS_H

#include "HyperNeat.hpp"
#include <vector>
using namespace std;
using namespace ANN_USM;

#define SIGMOID_CONST 4.9 

namespace ANN_USM{

	double OutputNodeFunction(double input);
	double Sigmoid(double input);
	//double USER_CUSTOM(double input);
	void UserTrain(HyperNeat * hyperneat, vector < double * > inputs, vector < double * > outputs);

}
#endif