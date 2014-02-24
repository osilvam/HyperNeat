#ifndef HYPERNEAT_CPP
#define HYPERNEAT_CPP

#include "HyperNeat.hpp"

#include <unistd.h>

using namespace ANN_USM;

HyperNeat::HyperNeat(vector < double * > inputs, vector < double * > outputs, string hyperneat_info){
	substrate = new Substrate(inputs,outputs);
	//cppn = new ConnectiveCPPN;	
	//cout << "INICIO HYPERNEAT"<< endl;
	HJsonDeserialize(hyperneat_info);
	cout << "TERMINO DESERIALIZE"<< endl;
}
HyperNeat::~HyperNeat(){
	free(substrate);
	vector < CPPNInputs >().swap(AditionalCPPNInputs);
	vector < double >().swap(CppnInputs);
	vector < vector < SpatialConnection > >().swap(connections);
}
void HyperNeat::HJsonDeserialize(string hyperneat_info){
	char str[(int)hyperneat_info.size()];
	strcpy(str, hyperneat_info.c_str());
	const char delimeters[] = "{\"\t\n:,[ ]}";
	char *pch = strtok(str, delimeters);
	while(pch != NULL){
		if (!strcmp(pch,(char *)"Substrate")){	
				pch = strtok(NULL, delimeters);
				substrate->SJsonDeserialize(pch);
				pch = strtok(NULL, delimeters);
		}else{
			if(!strcmp(pch,(char *)"connection_threshold")){
				pch = strtok(NULL, delimeters);
				connection_threshold = atoi(pch);
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
void HyperNeat::CreateSubstrateConnections(){
	if(substrate->GetLayoutNumber() < 1){
		cout << "Does not exist any substrate initialized" << endl;
		return;
	}
	n_connections = 0;
	if (substrate->GetLayoutNumber() > 1){
		for(int i = 0; i < substrate->GetLayoutNumber()-1; i++){
			vector < int > n_layer_nodesS = substrate->GetLayerNodesNumber(i);
			vector < int > n_layer_nodesT = substrate->GetLayerNodesNumber(i+1);
			vector < SpatialConnection > aux;
			for(int j = 0; j < substrate->GetLayersNumber(i); j++){
				for(int k = 0; k < n_layer_nodesS[j]; k++){
					for(int r = 0; r < substrate->GetLayersNumber(i+1); r++){
						for(int s = 0; s < n_layer_nodesT[r]; s++){
							vector < double > cppn_inputs;
							vector < double > c1 = (substrate->GetSpatialNode(i,j,k))->GetCoordenates();
							vector < double > c2 = (substrate->GetSpatialNode(i+1,r,s))->GetCoordenates();
							cppn_inputs.insert(cppn_inputs.end(), c1.begin(), c1.end());
							cppn_inputs.insert(cppn_inputs.end(), c2.begin(), c2.end());
							vector < double > input_aux (cppn_inputs);
							for(int c = 0; c < n_AditionalCPPNInputs; c++)
								cppn_inputs.push_back(AditionalCPPNInputs[c].Eval(input_aux));
							//AGREGAR CALCULO WEIGHT
							double weight = 1;
							if(weight > connection_threshold){
								aux.push_back(SpatialConnection(substrate->GetSpatialNode(i,j,k),substrate->GetSpatialNode(i+1,r,s),weight));
								n_connections++;
							}
						}
					}
				}
			}
			connections.push_back(aux);
		}
	}else{		
		vector < int > n_layer_nodes = substrate->GetLayerNodesNumber(0);
		for(int i = 0; i < substrate->GetLayersNumber(0)-1; i++){			
			vector < SpatialConnection > aux;
			for(int j = 0; j < n_layer_nodes[i]; j++){
				for(int k = 0; k < n_layer_nodes[i+1]; k++){
					vector < double > cppn_inputs;
					vector < double > c1 = (substrate->GetSpatialNode(0,i,j))->GetCoordenates();
					vector < double > c2 = (substrate->GetSpatialNode(0,i+1,k))->GetCoordenates();
					cppn_inputs.insert(cppn_inputs.end(), c1.begin(), c1.end());
					cppn_inputs.insert(cppn_inputs.end(), c2.begin(), c2.end());
					vector < double > input_aux (cppn_inputs);
					for(int c = 0; c < n_AditionalCPPNInputs; c++)
						cppn_inputs.push_back(AditionalCPPNInputs[c].Eval(input_aux));
					//AGREGAR CALCULO WEIGHT
					double weight = 1;
					if(weight > connection_threshold){
						aux.push_back(SpatialConnection(substrate->GetSpatialNode(0,i,j),substrate->GetSpatialNode(0,i+1,k),weight));
						n_connections++;
					}
				}
			}
			connections.push_back(aux);
		}
	}
}
void HyperNeat::EvaluateSubstrateConnections(){
	if(substrate->GetLayoutNumber() < 1){
		cout << "Does not exist any substrate initialized" << endl;
		return;
	}
	if(n_connections < 1){		
		cout << "Does not exist any connection initialized" << endl;
		return;
	}
	int i;
	if(substrate->GetLayoutNumber() > 1){
		for(i = 0; i < substrate->GetLayoutNumber()-1; i++){
			substrate->ClearSpatialNodeInputs(i+1);
			substrate->EvaluateSpatialNode(i);
			EvaluateConnections(i);
		}
		substrate->EvaluateSpatialNode(i);
	}else{		
		for(i = 0; i < substrate->GetLayersNumber(0)-1;i++){
			substrate->ClearSpatialNodeInputs(0,i+1);
			substrate->EvaluateSpatialNode(0,i);
			EvaluateConnections(i);			
		}
		//substrate->EvaluateSpatialNode(0,i);
		substrate->EvaluateSpatialNode(0,i);
	}

}
void HyperNeat::EvaluateConnections(int layer_num){
	for(int i = 0; i < (int)connections[layer_num].size(); i++)
		connections[layer_num][i].Evaluate();
}
void HyperNeat::PrintInputs(){
	substrate->PrintInputs();
}
#endif