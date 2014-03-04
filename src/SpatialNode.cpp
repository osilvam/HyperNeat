#ifndef SPATIALNODE_CPP
#define SPATIALNODE_CPP

#include "SpatialNode.hpp"
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
	vector < double * >().swap(inputs);
}
void SpatialNode::SetInputToInputNode(double * input){
	if (node_type != 0){
		cout << "This node is not of type input" << endl;
		return;
	}
	this->input = input;
}
void SpatialNode::SetOutputToOutputNode(double * output){
	if (node_type != 2){
		cout << "This node is not of type output" << endl;
		return;
	}
	this->output = output;
}
void SpatialNode::AddInputToNode(double * input){
	inputs.push_back(new double);
	n_inputs++;
	inputs[n_inputs-1] = input;
}
double * SpatialNode::AddOutputToNode(){
	return this->output;
}
void SpatialNode::OutputCalcule(){
	double aux = (node_type == 0) ? *input : 0.0;
	cout << "ID: " << id;
	if(node_type == 0) cout << " Input_puntero: " << *input;
	cout <<" | Inputs_nodos ( ";
	for(int i = 0; i < n_inputs; i++){
		aux += *inputs[i];
		cout << *inputs[i] << " ";
	}
	*output = aux;
	cout <<") | Output_puntero: " << output << endl;
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
	for(int i = 0; i < (int)inputs.size(); i++)
		delete inputs[i];
	inputs.clear();
	n_inputs = 0;
}

#endif