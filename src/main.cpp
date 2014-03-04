
#include "main.hpp"
#include <iostream>


int main(int argc, char *argv[]){
	
	string jsonstring = "{"
	"\"n_AditionalCPPNInputs\": \"3\","
	"\"AditionalCPPNInputs\": [ \"BIAS\": \"0.9\", \"RADIUS1\", \"EUCLIDIANDISTANCE\" ],"
	"\"connection_threshold\": \"0.5\","
	"\"Substrate\":"
	"{"
	"\"n_layouts\": \"1\","
	"\"Layouts\":"
	"["
	"{"
	"\"coordenate_type\": \"0\","
	"\"n_layers\": \"3\","
	"\"n_layer_nodes\": [\"3\", \"3\",\"3\" ],"
	"\"nodes_info\":"
	"["
	"["
	"[ \"0\", \"0\", \"0\" ],"
	"[ \"0\", \"1\", \"0\" ],"
	"[ \"0\", \"2\", \"0\" ]"
	"],"
	"["
	"[ \"1\", \"-1\", \"1\" ],"
	"[ \"1\", \"-1\", \"1\" ],"
	"[ \"1\", \"-1\", \"1\" ]"
	"],"
	"["
	"[ \"2\", \"0\", \"2\" ],"
	"[ \"2\", \"1\", \"2\" ],"
	"[ \"2\", \"2\", \"2\" ]"
	"]"
	"],"
	"\"nodes_coordenate\":"
	"["
	"["
	"[ \"-1.0\", \"1.0\" ],"
	"[ \"-1.0\", \"0.0\" ],"
	"[ \"-1.0\", \"-1.0\" ]"
	"],"
	"["
	"[ \"0.0\", \"1.0\" ],"
	"[ \"0.0\", \"0.0\" ],"
	"[ \"0.0\", \"-1.0\" ]"
	"],"
	"["
	"[ \"1.0\", \"1.0\" ],"
	"[ \"1.0\", \"0.0\" ],"
	"[ \"1.0\", \"-1.0\" ]"
	"]"
	"]"
	"}"
	"]"
	"}"
	"}";

	vector < double * > inputs;
	vector < double * > outputs;

	for(int i = 0; i < 3; i++){
		double * input = new double(0.5 + 0.1*i);
		double * output = new double(0.0);
		inputs.push_back(input);
		outputs.push_back(output);
	}

	cout << "EMPEZO EL PROGRAMA -------------------------------" << endl;

	HyperNeat * hyperneat = new HyperNeat(inputs, outputs, jsonstring);

	int trains = 2;
	for(int i = 0; i < trains; i++)
		UserTrain(hyperneat, inputs, outputs);
		
	vector<double*>().swap(inputs);
	vector<double*>().swap(outputs);

	cout << "TERMINO EL PROGRAMA ------------------------------" << endl;

	return 0;
}

