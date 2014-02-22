#ifndef HYPERNEAT_CPP
#define HYPERNEAT_CPP

#include "HyperNeat.hpp"

#include <unistd.h>

using namespace ANN_USM;

HyperNeat::HyperNeat(vector < double * > inputs, vector < double * > outputs, string hyperneat_info){
	this->inputs = inputs;
	this->outputs = outputs;
	//cppn = new ConnectiveCPPN;	
	//cout << "INICIO HYPERNEAT"<< endl;
	HJsonDeserialize(hyperneat_info);
	cout << "TERMINO DESERIALIZE"<< endl;
	/*CreateSubstrateConnections();
	cout << "TERMINO CREATE" << endl;
	EvaluateSubstrateConnections();
	cout << "TERMINO EVALUATE" << endl;*/
}
HyperNeat::~HyperNeat(){
	vector < CPPNInputs >().swap(AditionalCPPNInputs);
	vector < Substrate >().swap(substrates);
	vector < double >().swap(CppnInputs);
	vector < vector < SpatialConnection > >().swap(connections);
	vector < double * >().swap(inputs);
	vector < double * >().swap(outputs);
}
void HyperNeat::HJsonDeserialize(string hyperneat_info){
	char * str;
	strcpy(str, hyperneat_info.c_str());
	const char delimeters[] = "{\"\t\n:,[ ]}";
	char *pch = strtok(str, delimeters);
	//int count = 0;
	while(pch != NULL){
		//count++;
		if (!strcmp(pch,(char *)"Substrates")){
			for(int i = 0; i < n_substrates; i++){				
				pch = strtok(NULL, delimeters);
				substrates.push_back(Substrate(inputs, outputs));
				substrates[i].SJsonDeserialize(pch);
			}
			cout << "BREAK" << endl;
			break;
		}else{
			if(!strcmp(pch,(char *)"connection_threshold")){
				pch = strtok(NULL, delimeters);
				connection_threshold = atoi(pch);
				pch = strtok(NULL, delimeters);
			}else{
				if (!strcmp(pch,(char *)"n_substrates")){
					pch = strtok(NULL, delimeters);
					n_substrates = atoi(pch);
					//substrates.assign(n_substrates, Substrate(inputs, outputs));
					pch = strtok(NULL, delimeters);
				}else{
					if(!strcmp(pch,(char *)"AditionalCPPNInputs")){
						for(int i = 0; i < n_AditionalCPPNInputs; i++){
							pch = strtok(NULL, delimeters);		
							if (!strcmp(pch,(char *)"BIAS")){	
								char * aux = pch;		
								pch = strtok(NULL, delimeters);						
								AditionalCPPNInputs.push_back(CPPNInputs(aux, atof(pch)));
							}else																
								AditionalCPPNInputs.push_back(CPPNInputs(pch, 0.0));
						}						
						pch = strtok(NULL, delimeters);					
					}else{
						if(!strcmp(pch,(char *)"n_AditionalCPPNInputs")){
							pch = strtok(NULL, delimeters);
							n_AditionalCPPNInputs = atoi(pch);
							pch = strtok(NULL, delimeters);
						}
					}		
				}
			}						
		}
	}
}
void HyperNeat::CreateSubstrateConnections(){
	if(n_substrates < 1){
		cout << "Does not exist any substrate initialized" << endl;
		return;
	}
	n_connections = 0;
	if (n_substrates > 1){
		vector < vector < SpatialConnection > > aux2;
		for(int i = 0; i < n_substrates-1; i++){
			vector <  SpatialConnection > aux1;
			for(int j = 0; j < (substrates[i].GetLayerNodesNumber())[0]; j++){
				for(int k = 0; k < (substrates[i+1].GetLayerNodesNumber())[0]; k++){
					vector < double > cppn_inputs;
					vector < double > c1 = (substrates[i].GetSpatialNode(0,j)).GetCoordenates();
					vector < double > c2 = (substrates[i+1].GetSpatialNode(0,k)).GetCoordenates();
					cppn_inputs.insert(cppn_inputs.end(), c1.begin(), c1.end());
					cppn_inputs.insert(cppn_inputs.end(), c2.begin(), c2.end());
					vector < double > aux (cppn_inputs);
					for(int c = 0; c < n_AditionalCPPNInputs; c++)
						cppn_inputs.push_back(AditionalCPPNInputs[c].Eval(aux));
					//AGREGAR EL CALCULO DE EL PESO!!!!!PASANDOLE cppn_inputs
					double weight = 0;
					if(weight > connection_threshold){
						aux1.push_back(SpatialConnection(substrates[i].GetSpatialNode(0,j), substrates[i+1].GetSpatialNode(0,k), weight));						}
						n_connections++;
					}
			}
			aux2.push_back(aux1);
		}
		connections = aux2;
	}else{
		vector < int > n_layer_nodes (substrates[0].GetLayerNodesNumber());		
		vector < vector < SpatialConnection > > aux2;
		for(int i = 0; i < substrates[0].GetLayersNumber()-1; i++){
			vector <  SpatialConnection > aux1;
			for(int j = 0; j < n_layer_nodes[i]; j++){
				for(int k = 0; k < n_layer_nodes[i+1]; k++){vector < double > cppn_inputs;
					vector < double > c1 = (substrates[0].GetSpatialNode(i,j)).GetCoordenates();
					vector < double > c2 = (substrates[0].GetSpatialNode(i+1,k)).GetCoordenates();
					cppn_inputs.insert(cppn_inputs.end(), c1.begin(), c1.end());
					cppn_inputs.insert(cppn_inputs.end(), c2.begin(), c2.end());
					vector < double > aux (cppn_inputs);
					for(int c = 0; c < n_AditionalCPPNInputs; c++)
						cppn_inputs.push_back(AditionalCPPNInputs[c].Eval(aux));					
					//AGREGAR EL CALCULO DE EL PESO!!!!!PASANDOLE cppn_inputs
					double weight = 0;
					if(weight > connection_threshold){		
						aux1.push_back(SpatialConnection(substrates[0].GetSpatialNode(i,j), substrates[0].GetSpatialNode(i+1,k), weight));					}
						n_connections++;
					}
			}
			aux2.push_back(aux1);
		}		
		connections = aux2;
	}
}
void HyperNeat::EvaluateSubstrateConnections(){
	if(n_substrates < 1){
		cout << "Does not exist any substrate initialized" << endl;
		return;
	}
	if(n_connections < 1){		
		cout << "Does not exist any connection initialized" << endl;
		return;
	}
	if(n_substrates > 1){
		int i;
		for(i = 0; i < n_substrates-1; i++){
			for(int j = 0; j < (substrates[i].GetLayerNodesNumber())[0]; j++)
				substrates[i].EvaluateSpatialNode(0,j);					
			for(int j = 0; j < (int)connections[i].size(); j++)
				connections[i][j].Evaluate();
		}
		for(int j = 0; j < (substrates[i].GetLayerNodesNumber())[0]; j++)
			substrates[i].EvaluateSpatialNode(0,j);					
	}else{		
		vector < int > n_layer_nodes (substrates[0].GetLayerNodesNumber());
		int i;	
		for(i = 0; i < substrates[0].GetLayersNumber()-1; i++){
			for(int j = 0; j < n_layer_nodes[i]; j++)
				substrates[0].EvaluateSpatialNode(i,j);
			for(int j = 0; j < (int)connections[i].size(); j++)
				connections[i][j].Evaluate();
		}
		for(int j = 0; j < n_layer_nodes[i]; j++)
			substrates[0].EvaluateSpatialNode(i,j);
	}

}
#endif