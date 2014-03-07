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
		return (double)(1 / (1 + exp(-SIGMOID_CONST*input)));
	}
	/*
	double USER_CUSTOM(double input){
		return (double)(USER_FUNCTION);
	}
	*/
	void UserTrain(HyperNeat * hyperneat, vector < double * > inputs, vector < double * > outputs){
		
		//vector < vector < double > > outputs_set;
		double I0[4] = {0.0,0.0,1.0,1.0};
		double I1[4] = {0.0,1.0,0.0,1.0};

		double error_sum;

		int test = 4;//Necesary to set the test number		

		clog << "\t-> Training    ";

		int porsentage = 0;
		for (int g = 0; g < GENERATIONS; g++)
		{
			porsentage = ((double)g / (GENERATIONS - 1)) * 100;

			clog << "\b";
			for (int i = porsentage; i > 0 ; i/=10)
				clog << "\b";

			clog << porsentage << "\%";

			for(int p = 0; p < POPULATION_MAX; p++)
			{
				vector < double > aux;
				error_sum = 0;
				bool flag = false;

				for(int i = 0; i < test; i++)
				{
					//The user can modify the HyperNeat inputs (vector < double * > inputs) for each test
					*inputs[0] = I0[i];
					*inputs[1] = I1[i];

					//cout << "EMPEZO EL CREATECONNECTIONS ----------------------" << endl;
					hyperneat->CreateSubstrateConnections(p);//Necesary to create connections
					//cout << "==================================================" << endl;
					//cout << "EMPEZO EL EVALUATE -------------------------------" << endl;
					flag = hyperneat->EvaluateSubstrateConnections();//Necesary to evaluate Substrate connections
					//cout << "==================================================" << endl;
					
					//PRINT INPUTS
					//for(int j = 0; j < (int)inputs.size(); j++)
					//	cout << *inputs[j] << endl;
					//PRINT OUTPUTS
					//for(int j = 0; j < (int)outputs.size(); j++)
					//	cout << *outputs[j] << endl;

					for(int j = 0; j < (int)outputs.size(); j++)
						aux.push_back(*outputs[j]);
				}

				if(!flag) continue;

				error_sum += abs(aux[0]);
				error_sum += 1 - aux[1];
				error_sum += 1 - aux[2];
				error_sum += abs(aux[3]);
				
				// ------------------------- FITNESS ------------------------- //
				// In this session the user must add its own fitness function
				// Furthermore at the end of the section should call the following function:
				double fitness = pow(4 - error_sum, 2);

				cout << endl << "===========================================" << endl;
				cout << "Aux: " << aux[0] << " | " << aux[1] << " | " << aux[2] << " | " << aux[3] << endl;
				cout << "Error_sum: " << error_sum << endl;
				cout << "Fitness: " << fitness << endl;

				hyperneat->HyperNeatFitness(fitness, p);
				// =========================================================== //
			}

			hyperneat->HyperNeatEvolve();
		}

		clog << " ok!" << endl;

		// PRINTING THE BEST RESULT

		clog << endl << "BEST RESULT ------------------------------------" << endl << endl;
		clog << "\t-> " << hyperneat->cppn_neat->champion.fitness << endl << endl;
	}
}

#endif