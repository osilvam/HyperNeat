#ifndef SUBSTRATE_CPP
#define SUBSTRATE_CPP

#include "Substrate.hpp"
using namespace ANN_USM;

Substrate::Substrate(vector < double * > inputs, vector < double * > outputs){
	this->inputs = inputs;
	this->outputs = outputs;
}
Substrate::~Substrate(){
	vector < int >().swap(n_layer_nodes);
	vector < vector < vector < int > > >().swap(nodes_info);
	vector < vector < vector < double > > >().swap(nodes_coordenate);
	vector < double * >().swap(inputs);
	vector < double * >().swap(outputs);
	vector < vector < SpatialNode > >().swap(nodes);
}
void Substrate::SJsonDeserialize(char * substrate_info){
	//char *str;
	//strcpy(str, substrate_info.c_str());
	const char delimeters[] = "{\"\t\n:,[ ]}";
	//char *pch = strtok(str, delimeters);
	while(substrate_info != NULL){
		if (!strcmp(substrate_info,(char *)"nodes_coordenate")){
			vector < vector < vector < double > > > aux3;
			for(int i = 0; i < n_layers; i++){
				vector < vector < double > > aux2;
				for(int j = 0; j < n_layer_nodes[i]; j++){
					vector < double > aux1;
					for(int k = 0; k < 2 + coordenate_type%2; k++){
						substrate_info = strtok(NULL, delimeters);
						aux1.push_back(atof(substrate_info));
					}
					aux2.push_back(aux1);
				}
				aux3.push_back(aux2);
			}
			nodes_coordenate = aux3;
			break;
		}else{
			if (!strcmp(substrate_info,(char *)"nodes_info")){
				vector < vector < vector < int > > > aux3;
				for(int i = 0; i < n_layers; i++){
					vector < vector < int > > aux2;
					for(int j = 0; j < n_layer_nodes[i]; j++){
						vector < int > aux1;
						for(int k = 0; k < 3; k++){
							substrate_info = strtok(NULL, delimeters);
							aux1.push_back(atoi(substrate_info));
						}
						aux2.push_back(aux1);
					}
					aux3.push_back(aux2);
				}
				nodes_info = aux3;
				substrate_info = strtok(NULL, delimeters);
			}else{
				if (!strcmp(substrate_info,(char *)"n_layer_nodes")){
					for(int i = 0; i < n_layers; i++){
						substrate_info = strtok(NULL, delimeters);
						n_layer_nodes.push_back(atoi(substrate_info));
					}					
					substrate_info = strtok(NULL, delimeters);
				}else{
					if (!strcmp(substrate_info,(char *)"n_layers")){
						substrate_info = strtok(NULL, delimeters);
						n_layers = atoi(substrate_info);
						substrate_info = strtok(NULL, delimeters);
					}else{
						if (!strcmp(substrate_info,(char *)"coordenate_type")){
							substrate_info = strtok(NULL, delimeters);
							coordenate_type = atoi(substrate_info);
							substrate_info = strtok(NULL, delimeters);
						}else{
							substrate_info = strtok(NULL, delimeters);
						}
					}
				}
			}
		}
		
	}
	CreateNodes();
}
void Substrate::CreateNodes(){	
	vector < vector < SpatialNode > > aux2;
	for(int i = 0; i < n_layers; i++){
		vector < SpatialNode > aux1;
		for(int j = 0; j < n_layer_nodes[i]; j++){
			aux1.push_back(SpatialNode(nodes_info[i][j][0], nodes_info[i][j][2], nodes_coordenate[i][j]));
			if(nodes_info[i][j][0] == 0)
				aux1[j].SetInputToInputNode(inputs[nodes_info[i][j][1]]);
			else
				if (nodes_info[i][j][0] == 2){
					aux1[j].SetOutputToOutputNode(outputs[nodes_info[i][j][1]]);
				}
		}
		aux2.push_back(aux1);
	}
	nodes = aux2;
}
int Substrate::GetCoordenateType(){
	return coordenate_type;
}
int Substrate::GetLayersNumber(){
	return n_layers;
}
vector < int > Substrate::GetLayerNodesNumber(){
	return n_layer_nodes;
}
SpatialNode Substrate::GetSpatialNode(int layer_num, int layer_node_num){
	return nodes[layer_num][layer_node_num];
}
void Substrate::EvaluateSpatialNode(int layer_num, int layer_node_num){
	nodes[layer_num][layer_node_num].OutputCalcule();
}

#endif