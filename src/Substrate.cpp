#ifndef SUBSTRATE_CPP
#define SUBSTRATE_CPP

#include "Substrate.hpp"
#include "UserFunctions.hpp"
using namespace ANN_USM;

Substrate::Substrate(vector < double * > inputs, vector < double * > outputs){
	this->inputs = inputs;
	this->outputs = outputs;	
}
Substrate::Substrate(){

}
Substrate::~Substrate(){
	vector < int >().swap(coordinate_type);
	vector < int >().swap(n_layers);
	vector < vector < int > >().swap(n_layer_nodes);
	vector < vector < vector < vector < int > > > >().swap(nodes_info);
	vector < vector < vector < vector < double > > > >().swap(nodes_coordinate);
	vector < double * >().swap(inputs);
	vector < double * >().swap(outputs);
	vector < vector < SpatialNode * > >().swap(nodes);
}
char * Substrate::SJsonDeserialize(char * substrate_info){
	//char *str;
	//strcpy(str, substrate_info.c_str());
	const char delimeters[] = "{\"\t\n:,[ ]}";
	//char *pch = strtok(str, delimeters);
	while(substrate_info != NULL){
		if (!strcmp(substrate_info,(char *)"Layouts")){							
			substrate_info = strtok(NULL, delimeters);
			for(int i = 0; i < n_layouts; i++){
				while(substrate_info != NULL){
					if (!strcmp(substrate_info,(char *)"nodes_coordinate")){					
						vector < vector < vector < double > > > aux1;
						for(int j = 0; j < n_layers[i]; j++){
							vector < vector < double > > aux2;
							for(int k = 0; k < n_layer_nodes[i][j]; k++){
								vector < double > aux3;
								for(int t = 0; t < 2+coordinate_type[i]%2; t++){								
									substrate_info = strtok(NULL, delimeters);
									aux3.push_back(atof(substrate_info));
								}
								aux2.push_back(aux3);
							}
							aux1.push_back(aux2);
						}
						nodes_coordinate.push_back(aux1);
						substrate_info = strtok(NULL, delimeters);
						break;
					}else{
						if (!strcmp(substrate_info,(char *)"nodes_info")){						
							vector < vector < vector < int > > > aux1;
							for(int j = 0; j < n_layers[i]; j++){
								vector < vector < int > > aux2;
								for(int k = 0; k < n_layer_nodes[i][j]; k++){
									vector < int > aux3;
									for(int t = 0; t < 3; t++){
										substrate_info = strtok(NULL, delimeters);
										aux3.push_back(atoi(substrate_info));
									}
									aux2.push_back(aux3);
								}
								aux1.push_back(aux2);
							}
							nodes_info.push_back(aux1);
							substrate_info = strtok(NULL, delimeters);
						}else{
							if (!strcmp(substrate_info,(char *)"n_layer_nodes")){						
								vector < int > aux;
								for(int j = 0; j < n_layers[i]; j++){
									substrate_info = strtok(NULL, delimeters);
									aux.push_back(atoi(substrate_info));
								}
								n_layer_nodes.push_back(aux);
								substrate_info = strtok(NULL, delimeters);
							}else{
								if (!strcmp(substrate_info,(char *)"n_layers")){	
									substrate_info = strtok(NULL, delimeters);
									n_layers.push_back(atoi(substrate_info));
									substrate_info = strtok(NULL, delimeters);
								}else{
									if (!strcmp(substrate_info,(char *)"coordinate_type")){						
										substrate_info = strtok(NULL, delimeters);
										coordinate_type.push_back(atoi(substrate_info));
										substrate_info = strtok(NULL, delimeters);
									}
								}
							}
						}
					}
				}
			}
			break;
		}else{
			if (!strcmp(substrate_info,(char *)"n_layouts")){						
				substrate_info = strtok(NULL, delimeters);
				n_layouts = atoi(substrate_info);
				substrate_info = strtok(NULL, delimeters);
			}
		}		
	}
	CreateNodes();
	return substrate_info;
}
void Substrate::CreateNodes(){
	int id  = 0;
	if(n_layouts > 1){
		for(int i = 0; i < n_layouts; i++){
			vector < SpatialNode * > aux1; 
			for(int j = 0; j < n_layer_nodes[i][0]; j++){
				aux1.push_back(new SpatialNode(id, nodes_info[i][0][j][0], nodes_info[i][0][j][2], nodes_coordinate[i][0][j]));
				if(nodes_info[i][0][j][0] == 0)
					aux1[j]->SetInputToInputNode(inputs[nodes_info[i][0][j][1]], nodes_info[i][0][j][1]);
				else
					if(nodes_info[i][0][j][0] == 2)
						aux1[j]->SetOutputToOutputNode(outputs[nodes_info[i][0][j][1]], nodes_info[i][0][j][1]);
				id++;
			}
			nodes.push_back(aux1);
		}

	}else{
		for(int i = 0; i < n_layers[0]; i++){
			vector < SpatialNode * > aux1; 
			for(int j = 0; j < n_layer_nodes[0][i]; j++){
				aux1.push_back(new SpatialNode(id, nodes_info[0][i][j][0], nodes_info[0][i][j][2], nodes_coordinate[0][i][j]));
				if(nodes_info[0][i][j][0] == 0)
					aux1[j]->SetInputToInputNode(inputs[nodes_info[0][i][j][1]], nodes_info[0][i][j][1]);
				else
					if(nodes_info[0][i][j][0] == 2)
						aux1[j]->SetOutputToOutputNode(outputs[nodes_info[0][i][j][1]], nodes_info[0][i][j][1]);
				id++;
			}
			nodes.push_back(aux1);
		}
	}
}
int Substrate::GetLayoutNumber(){
	return n_layouts;
}
int Substrate::GetCoordinateType(int layout_num){
	return coordinate_type[layout_num];
}
int Substrate::GetLayersNumber(int layout_num){
	return n_layers[layout_num];
}
int Substrate::GetLayerNodesNumber(int layout_num, int layer_num){
	return n_layer_nodes[layout_num][layer_num];
}
SpatialNode * Substrate::GetSpatialNode(int layout_num, int layer_num, int layer_node_num){
	if(n_layouts > 1)
		return nodes[layout_num][layer_node_num];
	else
		return nodes[layer_num][layer_node_num];
}
void Substrate::EvaluateSpatialNode(int layout_num, int layer_num){
	//cout << "OutputCalcule - ids ( " << endl;
	if(n_layouts > 1)
		for(int j = 0; j < n_layer_nodes[layout_num][0]; j++){
			nodes[layout_num][j]->OutputCalcule();
		}
	else
		for(int j = 0; j < n_layer_nodes[0][layer_num]; j++){
			nodes[layer_num][j]->OutputCalcule();
		}
	//cout << " ) " << endl;
}
void Substrate::ClearSpatialNodeInputs(int layout_num, int layer_num){
	//cout << "ClearInputs - ids ( ";

	if(n_layouts > 1)
		for(int j = 0; j < n_layer_nodes[layout_num][0]; j++){
			//cout << nodes[layout_num][j]->GetId() << " ";
			nodes[layout_num][j]->ClearInputs();
		}
	else
		for(int j = 0; j < n_layer_nodes[0][layer_num]; j++){
			//cout << nodes[layer_num][j]->GetId() << " ";
			nodes[layer_num][j]->ClearInputs();
		}
	//cout << ") " << endl;
}
double Substrate::GetSpatialNodeOutput(int layout_num, int layer_num, int layer_node_num){
	if(n_layouts > 1)
		return nodes[layout_num][layer_node_num]->GetOuput();
	else
		return nodes[layer_num][layer_node_num]->GetOuput();
}
double Substrate::GetSpatialNodeId(int layout_num, int layer_num, int layer_node_num){
	if(n_layouts > 1)
		return nodes[layout_num][layer_node_num]->GetId();
	else
		return nodes[layer_num][layer_node_num]->GetId();
}
vector < string > Substrate::GetSubstrateOutputFunctions(string plataform){
	vector < string > functions;
	if(n_layouts > 1){
		for(int i = 0; i < n_layouts; i++)
			for(int j = 0; j < n_layer_nodes[i][0]; j++)
				if(nodes_info[i][0][j][0] == 2)
					functions.push_back(nodes[i][j]->GetNodeFunction(plataform));
	}else{
		for(int i = 0; i < n_layers[0]; i++)
			for(int j = 0; j < n_layer_nodes[0][i]; j++)
				if(nodes_info[0][i][j][0] == 2)
					functions.push_back(nodes[i][j]->GetNodeFunction(plataform));
	}
	return functions;
}

#endif