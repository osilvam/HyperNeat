#ifndef USERFUNCTIONS_CPP
#define USERFUNCTIONS_CPP

#include <iostream>
#include <sstream>
#include <string>
#include "UserFunctions.hpp"

namespace ANN_USM{
	
	string GetNodeFunctionInfo(){
		stringstream info;
		if(!strcmp(NODE_FUNCTION,(char *)"SIGMOID")) 
			info << "NODE_FUNCTION: " << NODE_FUNCTION << " - EXPRESION: " << SIGMOID_FUNC << " - SIGMOID_CONST: " << SIGMOID_CONST;
		//else if(!strcmp(NODE_FUNCTION,(char *)"USER_CUSTOM"))
		//	info << "USER_CUSTOM";
		return info.str();
	}

	double OutputNodeFunction(double input){
		return Sigmoid(input);//Necessary to indicate which node function used
		//return USER_CUSTOM(input);
	}
	double Sigmoid(double input){
		return (double)(1/(1+exp(-SIGMOID_CONST*input)));
	}
	/*
	double USER_CUSTOM(double input){
		return (double)(USER_FUNCTION);
	}
	*/
	void UserTrain(HyperNeat * hyperneat, vector < double * > inputs, vector < double * > outputs){
		
		vector < vector < double > > outputs_set;
		int test = 1;//Necesary to set the test number		

		for(int i = 0; i < test; i++){

			vector < double > aux;

			//The user can modify the HyperNeat inputs (vector < double * > inputs) for each test

			cout << "EMPEZO EL CREATECONNECTIONS ----------------------" << endl;
			hyperneat->CreateSubstrateConnections();//Necesary to create connections
			cout << "==================================================" << endl;
			cout << "EMPEZO EL EVALUATE -------------------------------" << endl;
			hyperneat->EvaluateSubstrateConnections();//Necesary to evaluate Substrate connections
			cout << "==================================================" << endl;
			
			//PRINT INPUTS
			for(int j = 0; j < (int)inputs.size(); j++)
				cout << *inputs[j] << endl;
			//PRINT OUTPUTS
			for(int j = 0; j < (int)outputs.size(); j++)
				cout << *outputs[j] << endl;

			for(int j = 0; j < (int)outputs.size(); j++)
				aux.push_back(*outputs[j]);

			outputs_set.push_back(aux);
		}
		
		// ------------------------- FITNESS ------------------------- //
		// In this session the user must add its own fitness function
		// Furthermore at the end of the section should call the following function:
		// hyperneat->HyperNeatFitness(fitness);
		// =========================================================== //

		//hyperneat->HyperNeatEvolve();

	}
}

#endif