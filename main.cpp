
#include "main.hpp"
#include <iostream>

int main(int argc, char *argv[]){
	
	string jsonstring = "{"
	"\"n_AditionalCPPNInputs\": \"3\","
	"\"AditionalCPPNInputs\": [ \"BIAS\": \"0.9\", \"RADIUS1\", \"EUCLIDIANDISTANCE\" ],"
	"\"n_substrates\": \"1\","
	"\"connection_threshold\": \"0.5\","
	"\"Substrates\":"
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
	"}";

	vector < double * > inputs;
	vector < double * > outputs;
	double * input = new double;
	double * output = new double;
	*input = 0.9;
	*output = 0.0;
	for(int i = 0; i < 9; i++){
		inputs.push_back(input);
		outputs.push_back(output);		
	}
	HyperNeat hyperneat (inputs, outputs, jsonstring);
	free(input);
	free(output);
	vector<double*>().swap(inputs);
	vector<double*>().swap(outputs);
	cout << "TERMINO EL PROGRAMA" << endl;
	return 0;
}

