
#include "main.hpp"
#include <iostream>

// Testing Program
// 3x3 board
int main(int argc, char *argv[])
{	
	//type, input/output id, sheet id

	string jsonstring = "{"
		"\"n_AditionalCPPNInputs\": \"0\","
		"\"AditionalCPPNInputs\":"
		"["			
		"],"
		"\"connection_threshold\": \"0.1\","
		"\"Substrate\":"
		"{"
			"\"n_layouts\": \"1\","
			"\"Layouts\":"
			"["
				"{"
					"\"coordinate_type\": \"0\","
					"\"n_layers\": \"3\","
					"\"n_layer_nodes\": [\"2\", \"3\",\"1\" ],"
					"\"nodes_info\":"
					"["
						"["
							"[ \"0\", \"0\", \"0\" ],"
							"[ \"0\", \"1\", \"0\" ]"
						"],"
						"["
							"[ \"1\", \"-1\", \"1\" ],"
							"[ \"1\", \"-1\", \"1\" ],"
							"[ \"1\", \"-1\", \"1\" ]"
						"],"
						"["
							"[ \"2\", \"0\", \"2\" ]"
						"]"
					"],"
					"\"nodes_coordinate\":"
					"["
						"["
							"[ \"-1.0\", \"1.0\" ],"
							"[ \"-1.0\", \"-1.0\" ]"
						"],"
						"["
							"[ \"0.0\", \"2.0\" ],"
							"[ \"0.0\", \"0.0\" ],"
							"[ \"0.0\", \"-2.0\" ]"
						"],"
						"["
							"[ \"1.0\", \"0.0\" ]"
						"]"
					"]"
				"}"
			"]"
		"}"
		"\"Genetic_Encoding\":"
		"{"
			"\"nodes\":"
			"["
				"{\"exist\": 1,\"node\": 0,\"type\": 0, \"row\": 0, \"function\": \"IDENTITY\"},"
				"{\"exist\": 1,\"node\": 1,\"type\": 0, \"row\": 0, \"function\": \"IDENTITY\"},"
				"{\"exist\": 1,\"node\": 2,\"type\": 0, \"row\": 0, \"function\": \"IDENTITY\"},"
				"{\"exist\": 1,\"node\": 3,\"type\": 0, \"row\": 0, \"function\": \"IDENTITY\"},"
				"{\"exist\": 1,\"node\": 4,\"type\": 2, \"row\": 1, \"function\": \"SPECIAL_SIGMOID\"}"
			"],"
			"\"connection_genes\":"
			"["
				"{\"exist\": 1,\"innovation\": 0,\"in\": 0,\"out\": 4,\"weight\": 1.,\"enable\": 1},"
				"{\"exist\": 1,\"innovation\": 1,\"in\": 1,\"out\": 4,\"weight\": 1.,\"enable\": 1},"
				"{\"exist\": 1,\"innovation\": 2,\"in\": 2,\"out\": 4,\"weight\": 1.,\"enable\": 1},"
				"{\"exist\": 1,\"innovation\": 3,\"in\": 3,\"out\": 4,\"weight\": 1.,\"enable\": 1}"
			"]"
		"}"
	"}";

	vector<double *> inputs;
	vector<double *> outputs;
	vector<string> FinalFunctions;

	for(int i = 0; i < 2; i++)
	{
		double * input = new double(0.0);

		inputs.push_back(input);
	}

	outputs.push_back(new double(0.0));

	cout << "EMPEZO EL PROGRAMA -------------------------------" << endl;

	HyperNeat * hyperneat = new HyperNeat(inputs, outputs, jsonstring);

	UserTrain(hyperneat, inputs, outputs);

	FinalFunctions = hyperneat->GetHyperNeatOutputFunctions();

	for(int i = 0; i < (int)FinalFunctions.size(); i++){
		cout << FinalFunctions[i] << endl;
	}
		
	vector<double*>().swap(inputs);
	vector<double*>().swap(outputs);

	cout << "TERMINO EL PROGRAMA ------------------------------" << endl;

	return 0;
}

