#ifndef SPATIALNODE_CPP
#define SPATIALNODE_CPP

#include "SpatialNode.hpp"
using namespace ANN_USM;

SpatialNode::SpatialNode(int id, int node_type, int layer_id, int layout_id, vector < double > coordenates){
	this->id = id;
	this->node_type = node_type;
	this->layer_id = layer_id;
	this->layout_id = layout_id;
	this->coordenates = coordenates;
	n_inputs = 0;
	output = new double(0.0);
	cout << "SpatialNode - id: " << id << " Output_puntero_dir: " << this->output << endl;
}
SpatialNode::SpatialNode(){
	
}
SpatialNode::~SpatialNode(){
	vector < double >().swap(coordenates);
	vector < double >().swap(inputs);
}
void SpatialNode::SetInputToInputNode(double * input){
	if (node_type != 0){
		cout << "This node is not of type input" << endl;
		return;
	}
	cout << "SetInputToInputNode: ";
	this->input = input;
	cout << id << " | ";
	cout << "Input_puntero: " << *this->input << " Input_puntero_dir: " << this->input <<endl;
}
void SpatialNode::SetOutputToOutputNode(double * output){
	if (node_type != 2){
		cout << "This node is not of type output" << endl;
		return;
	}
	//this->output = output;
	output = this->output;
}
void SpatialNode::AddInputToNode(double input){
	inputs.push_back(input);
	n_inputs++;
}
double * SpatialNode::AddOutputToNode(){
	//this->output = output;
	cout << "AddOutputToNode - id: " << id << " Output_puntero_dir: " << this->output << endl;
	return this->output;
}
void SpatialNode::OutputCalcule(){
	double aux = (node_type == 0) ? *input : 0.0;
	cout << "ID: " << id;
	if(node_type == 0) cout << " Input_puntero: " << *input << " Input_puntero_dir" <<  input;
	cout <<" | Inputs_nodos ( ";
	for(int i = 0; i < n_inputs; i++){
		aux += inputs[i];
		cout << inputs[i] << " ";
	}
	cout <<  ") Output_puntero_dir: " << output;
	*output = aux;
	cout <<" | Inputs_final: " << aux << " | Output_puntero: " << *output << endl;
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
int SpatialNode::GetLayerNodeId(){
	return layer_id;
}
int SpatialNode::GetLayoutNodeId(){
	return layout_id;
}
double SpatialNode::GetOuput(){
	cout <<  " | GETOUPUT: "<< *output << " | " << endl;
	return *output;
}
void SpatialNode::ClearInputs(){
	inputs.clear();
	n_inputs = 0;
}

#endif