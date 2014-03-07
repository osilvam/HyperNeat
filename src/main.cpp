
#include "main.hpp"
#include <iostream>
#include <fstream>

// Testing Program
// 3x3 board
int main(int argc, char *argv[])
{	
	srand (time(0));

	//type, input/output id, sheet id
	if(argc < 2) return -1;

	ifstream file;
	file.open(argv[1]);

	string jsonstring((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	vector<double *> inputs;
	vector<double *> outputs;

	vector<string> FinalFunctions;

	for(int i = 0; i < 2; i++)
	{
		double * input = new double(0.0);

		inputs.push_back(input);
	}

	outputs.push_back(new double(0.0));

	clog << endl << "PROGRAM STARTED -------------------------------" << endl << endl;

	HyperNeat * hyperneat = new HyperNeat(inputs, outputs, jsonstring);

	UserTrain(hyperneat, inputs, outputs);

	//FinalFunctions = hyperneat->GetHyperNeatOutputFunctions();

	//for(int i = 0; i < (int)FinalFunctions.size(); i++){
	//	cout << FinalFunctions[i] << endl;
	//}
		
	vector<double*>().swap(inputs);
	vector<double*>().swap(outputs);

	clog << "PROGRAM FINISHED ------------------------------" << endl << endl;

	return 0;
}

