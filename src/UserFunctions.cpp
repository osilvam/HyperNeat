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
			file_name << "functions_files/" << NODE_FUNCTION_TEXT << ".m";
			ofstream myfile (file_name.str().c_str());

			if(!strcmp(NODE_FUNCTION_TEXT,(char *)"SIGMOID")){

				if (myfile.is_open()){

			    	myfile << OCTAVE_SIGMOID_STATEMENT << endl;
				    myfile << OCTAVE_SIGMOID_CONST_LETTER << " = " << OCTAVE_SIGMOID_CONST << ";" << endl;
				    myfile << OCTAVE_SIGMOID_FUNC << ";" << endl;
				    myfile.close();

			  	}else 
			  		cerr << "Unable to open file: " << file_name.str() << endl;

			}
			//else if(!strcmp(NODE_FUNCTION_TEXT,(char *)"USER_CUSTOM"))
			//	info << "USER_CUSTOM";			
		}
		//else if(!strcmp(plataform.c_str(),(char *)"OTHER_PLATAFORM"))
		
	}

	double OutputNodeFunction(double input){
		return NODE_FUNCTION(input);
		//return Sigmoid(input);//Necessary to indicate which node function used
		//return USER_CUSTOM(input);
	}
	double Sigmoid(double input){
		return (double)((1 / (1 + exp(-SIGMOID_CONST*input)))-0.5)*2;
	}
	/*
	double USER_CUSTOM(double input){
		return (double)(USER_FUNCTION);
	}
	*/
	void UserTrain(HyperNeat * hyperneat, vector < double * > inputs, vector < double * > outputs){
		
		
	}
}

#endif