#ifndef USERFUNCTIONS_CPP
#define USERFUNCTIONS_CPP

#include <iostream>
#include <sstream>
#include <string>
#include "UserFunctions.hpp"

namespace ANN_USM{
	
	void GetNodeFunction(string plataform){

		if(!strcmp(plataform.c_str(),(char *)"octave")){

			stringstream file_name;
			file_name << "files/" << NODE_FUNCTION << ".m";
			ofstream myfile (file_name.str().c_str());

			if(!strcmp(NODE_FUNCTION,(char *)"SIGMOID")){

				if (myfile.is_open()){

			    	myfile << OCTAVE_SIGMOID_STATEMENT << endl;
				    myfile << OCTAVE_SIGMOID_CONST_LETTER << " = " << OCTAVE_SIGMOID_CONST << ";" << endl;
				    myfile << OCTAVE_SIGMOID_FUNC << ";" << endl;
				    myfile.close();

			  	}else 
			  		cerr << "Unable to open file: " << file_name.str() << endl;

			}
			//else if(!strcmp(NODE_FUNCTION,(char *)"USER_CUSTOM"))
			//	info << "USER_CUSTOM";			
		}
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
		
		// ============================ TEST INITIALIZATION ============================ //
		double I0[4] = {0.0,0.0,1.0,1.0};
		double I1[4] = {0.0,1.0,0.0,1.0};
		double error_sum;
		int test = 4;//Necesary to set the test number	
		int percentage = 0;
		bool flag = false;
		// ============================================================================= //


		clog << "\t-> Training    ";

		for (int g = 0; g < GENERATIONS; g++)
		{
			// ========== PRINTING COMPLETION PERCENTAGE ========== //
			percentage = ((double)g / (GENERATIONS - 1)) * 100;

			clog << "\b";
			for (int i = percentage; i > 0 ; i/=10)
				clog << "\b";

			clog << percentage << "\%";
			// ==================================================== //

			for(int p = 0; p < POPULATION_MAX; p++)
			{
				vector < double > aux;
				error_sum = 0;

				// ========================== TEST EVALUATION ========================== //
				for(int i = 0; i < test; i++)
				{
					//The user can modify the HyperNeat inputs (vector < double * > inputs) for each test
					*inputs[0] = I0[i];
					*inputs[1] = I1[i];

					hyperneat->CreateSubstrateConnections(p);//Necesary to create connections
					flag = hyperneat->EvaluateSubstrateConnections();//Necesary to evaluate Substrate connections
					
					for(int j = 0; j < (int)outputs.size(); j++)
						aux.push_back(*outputs[j]);
				}
				// ===================================================================== //



				if(!flag) continue;//If exist any error in substrate evaluation, the fitness is not evaluated



				// ============================== FITNESS ============================== //
				// In this session the user must add its own fitness function
				// Furthermore at the end of the section should call the following function:

				error_sum += abs(aux[0]);
				error_sum += 1 - aux[1];
				error_sum += 1 - aux[2];
				error_sum += abs(aux[3]);
								
				double fitness = pow(4 - error_sum, 2);

				cout << endl << "===========================================" << endl;
				cout << "Aux: " << aux[0] << " | " << aux[1] << " | " << aux[2] << " | " << aux[3] << endl;
				cout << "Error_sum: " << error_sum << endl;
				cout << "Fitness: " << fitness << endl;

				hyperneat->HyperNeatFitness(fitness, p);
				// ===================================================================== //
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