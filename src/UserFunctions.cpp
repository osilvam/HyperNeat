#ifndef USERFUNCTIONS_CPP
#define USERFUNCTIONS_CPP

#include <iostream>
#include "UserFunctions.hpp"
namespace ANN_USM{

	void Train(HyperNeat * hyperneat, vector < double * > inputs, vector < double * > outputs){
		
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