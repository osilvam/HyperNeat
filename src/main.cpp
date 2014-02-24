
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
	double * input1 = new double(0.6);
	inputs.push_back(input1);
	double * input2 = new double(0.5);
	inputs.push_back(input2);
	double * input3 = new double(0.4);
	inputs.push_back(input3);
	vector < double * > outputs;
	double * output1 = new double(0.3);
	outputs.push_back(output1);
	double * output2 = new double(0.2);
	outputs.push_back(output2);
	double * output3 = new double(0.1);
	outputs.push_back(output3);


	cout << "INICIO HYPERNEAT" << endl;
	HyperNeat hyperneat (inputs, outputs, jsonstring);

	cout << "EMPEZO EL CREATECONNECTIONS" << endl;
	hyperneat.CreateSubstrateConnections();
	cout << "TERMINO EL CREATECONNECTIONS" << endl;
	cout << "EMPEZO EL EVALUATE" << endl;
	hyperneat.EvaluateSubstrateConnections();
	cout << "TERMINO EL EVALUATE" << endl;

	for(int i = 0; i < (int)outputs.size(); i++)
		cout << *outputs[i] << " " << outputs[i] << endl;

	for(int i = 0; i < (int)inputs.size(); i++)
		*inputs[i] = *inputs[i] + 0.1;		

	//cout << "EMPEZO EL CREATECONNECTIONS" << endl;
	//hyperneat.CreateSubstrateConnections();
	//cout << "TERMINO EL CREATECONNECTIONS" << endl;
	cout << "EMPEZO EL EVALUATE" << endl;
	hyperneat.EvaluateSubstrateConnections();
	cout << "TERMINO EL EVALUATE" << endl;



	for(int i = 0; i < (int)outputs.size(); i++)
		cout << *outputs[i] << " " << outputs[i] << endl;

	vector<double*>().swap(inputs);
	vector<double*>().swap(outputs);
	cout << "TERMINO EL PROGRAMA" << endl;
	return 0;
}

