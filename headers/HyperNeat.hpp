#ifndef HYPERNEAT_H
#define HYPERNEAT_H

#include "Substrate.hpp"
#include "SpatialConnection.hpp"
//#include "ConnectiveCPPN.hpp"
#include "CPPNInputs.hpp"
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
using namespace std;

namespace ANN_USM{
	class HyperNeat
	{

		int n_AditionalCPPNInputs;//number of aditional cppn inputs	
		vector < CPPNInputs > AditionalCPPNInputs;//vector of aditional cppn inputs
		int connection_threshold;//threshold that determine the creation for an connection
		vector < double > CppnInputs;//vector of cppn input values
		vector < vector < SpatialConnection > > connections;//vector of overall connections
		int n_connections;//number of connections after evaluate connections with cppn-neat

		//ConnectiveCPPN * cppn;
		Substrate * substrate;//vector of overall HyperNeat substrates
	public:
		HyperNeat(vector < double * > inputs, vector < double * > outputs, string hyperneat_info);
		~HyperNeat();
		void HJsonDeserialize(string hyperneat_info);//Extract all HyperNeat information of char pointer
		void CreateSubstrateConnections();//Create all substrate connections according to cppn-neat result
		void EvaluateSubstrateConnections();//Allows to obtain the final HyperNeat outputs
		void EvaluateConnections(int layer_num);
	};
}
#endif