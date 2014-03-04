#ifndef USERFUNCTIONS_H
#define USERFUNCTIONS_H

#include "HyperNeat.hpp"
#include <vector>

using namespace std;

namespace ANN_USM{

	void Train(HyperNeat * hyperneat, vector < double * > inputs, vector < double * > outputs);

}
#endif