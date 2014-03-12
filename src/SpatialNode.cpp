#ifndef SPATIALNODE_CPP
#define SPATIALNODE_CPP

#include "SpatialNode.hpp"
#include "UserFunctions.hpp"
using namespace ANN_USM;

SpatialNode::SpatialNode(int id, int node_type, int sheet_id, vector < double > coordenates){
	this->id = id;
	this->node_type = node_type;
	this->sheet_id = sheet_id;
	this->coordenates = coordenates;
	n_inputs = 0;
	output = new double(0.0);
}
SpatialNode::SpatialNode(){
	
}
SpatialNode::~SpatialNode(){
	vector < double >().swap(coordenates);
	vector < double >().swap(inputs_weight);
	vector < SpatialNode * >().swap(inputs_nodes);
}
void SpatialNode::SetInputToInputNode(double * input, int input_id){
	if (node_type != 0){
		cout << "This node is not of type input" << endl;
		return;
	}
	this->input = input;
	this->input_id = input_id;
}
void SpatialNode::SetOutputToOutputNode(double * output, int output_id){
	if (node_type != 2){
		cout << "This node is not of type output" << endl;
		return;
	}
	this->output = output;
	this->output_id = output_id;
}
void SpatialNode::AddInputToNode(SpatialNode * input_node, double input_weight){
	if(node_type == 0){
		cout << "can not connect to a node of type input" << endl;
		return;
	}
	if (input_node->GetSheetNodeId() > sheet_id){			
		cout << "can not make a recurrent connection" << endl;
		return;
	}
	inputs_nodes.push_back(input_node);
	inputs_weight.push_back(input_weight);
	n_inputs++;
	//cout << "Input_ID: " << input_node->GetId() << " Output_ID: " << id;
	//cout << " Weight: " << input_weight << endl; 
}
double * SpatialNode::AddOutputToNode(){
	return this->output;
}
void SpatialNode::OutputCalcule(){
	double aux = (node_type == 0) ? *input : 0.0;
	//cout << "ID: " << id;
	//if(node_type == 0) cout << " Input_puntero: " << *input;
	//cout <<" | Inputs_nodos ( ";
	for(int i = 0; i < n_inputs; i++){
		aux += (inputs_nodes[i]->GetOuput())*inputs_weight[i];
		//cout << (inputs_nodes[i]->GetOuput())*inputs_weight[i] << " ";
	}
	*output = OutputNodeFunction(aux);
	//*output = aux;
	//cout <<") | Output_puntero: " << *output << endl;
}
vector < double > SpatialNode::GetCoordenates(){
	return coordenates;
}
int SpatialNode::GetId(){
	return id;
}
int SpatialNode::GetNodeType(){
	return node_type;
}
int SpatialNode::GetSheetNodeId(){
	return sheet_id;
}
double SpatialNode::GetOuput(){
	return *output;
}
void SpatialNode::ClearInputs(){	
	inputs_nodes.clear();
	inputs_weight.clear();
	n_inputs = 0;
}
string SpatialNode::GetNodeFunction(string plataform){
	stringstream function;
	if(node_type == 2) function << "OUTPUT_" << output_id << " = ";

	if(!strcmp(plataform.c_str(),(char *)"octave")){

		function << NODE_FUNCTION << "( ";

		if(node_type == 0) function << "INPUT_" << input_id;		
		else{
			if(n_inputs > 0)
				for(int i = 0; i < n_inputs; i++){
					function << inputs_nodes[i]->GetNodeFunction(plataform) << "* " << inputs_weight[i];
					if( i + 1 < n_inputs ) function << " + ";
				}
			else
				function << "0";
		}
			
		function << " ) ";
	}

	return function.str();
}
#endif