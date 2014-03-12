
#include "main.hpp"

int main(int argc, char *argv[])
{	
	srand (time(0));

	if(argc < 2) return -1;

	// ============================= READING JSON FILE ============================= //
	ifstream file;
	file.open(argv[1]);
	string jsonstring((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	// ============================================================================= //




	// ======================== INPUT INITIALIZATION EXAMPLE ======================= //
	vector<double *> inputs;
	vector<double *> outputs;

	vector<string> FinalFunctions;

	for(int i = 0; i < 2; i++)
	{
		double * input = new double(0.0);

		inputs.push_back(input);
	}

	outputs.push_back(new double(0.0));
	// ============================================================================= //




	// ============================= PROGRAM EXECUTION ============================= //
	clog << endl << "PROGRAM STARTED -------------------------------" << endl << endl;

	// HYPERNEAT OBJECT CREATED
	HyperNeat * hyperneat = new HyperNeat(inputs, outputs, jsonstring);

	// USER TRAIN FUNCTION
	UserTrain(hyperneat, inputs, outputs);

	cout << endl << "=========================================================" << endl;

	clog << "PROGRAM FINISHED ------------------------------" << endl << endl;
	// ============================================================================= //

	//OBTEIN FUNCTIONS
	hyperneat->GetHyperNeatOutputFunctions("octave");

	
	vector<double*>().swap(inputs);
	vector<double*>().swap(outputs);

	return 0;
}

