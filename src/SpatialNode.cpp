#ifndef SPATIALNODE_CPP
#define SPATIALNODE_CPP

#include "SpatialNode.hpp"
using namespace ANN_USM;

SpatialNode::SpatialNode(int id, int node_type, int layer_id, vector < double > coordenates){
	this->id = id;
	this->node_type = node_type;
	this->layer_id = layer_id;
	this->coordenates = coordenates;
	n_inputs = 0;
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
	output = this->output;
}
void SpatialNode::AddInputToNode(double input){
	inputs.push_back(&input);
	n_inputs++;
}
void SpatialNode::AddOutputToNode(double * output){
	output = this->output;
}
void SpatialNode::OutputCalcule(){
	double aux = node_type == 0 ? *input : 0.0;
	cout << "ID: " << id << " | " << node_type;
	if(node_type == 0) cout << " " << *input;
	cout << " aux_antes: " << aux;
	for(int i = 0; i < n_inputs; i++)
		aux += *inputs[i];
	output = &aux;
	cout << " | aux_despues: " << aux << " | output: " << GetOuput() << endl;
}
vector < double > SpatialNode::GetCoordenates(){
	return coordenates;
}
int SpatialNode::GetNodeType(){
	return node_type;
}
int SpatialNode::GetLayerNodeId(){
	return layer_id;
}
void SpatialNode::ClearInputs(){
	inputs.clear();
}
double SpatialNode::GetOuput(){
	return *output;
}
int SpatialNode::GetId(){//TEMPORAL
	return id;
}

#endif