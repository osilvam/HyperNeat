#ifndef SPATIALCONNECTION_CPP
#define SPATIALCONNECTION_CPP

#include "SpatialConnection.hpp"
using namespace ANN_USM;

SpatialConnection::SpatialConnection(SpatialNode * input_node, SpatialNode * output_node, double weight){
	if(output_node->GetNodeType() == 0){
		cout << "can not connect to a node of type input" << endl;
		return;
	}
	if (input_node->GetLayerNodeId() > output_node->GetLayerNodeId()){			
		cout << "can not make a recurrent connection" << endl;
		return;
	}
	this->input_node = input_node;
	this->output_node = output_node;
	this->weight = weight;
	input = new double;
	input = input_node->AddOutputToNode();
	cout << "SpatialConnection - input_id: " << input_node->GetId() << " Output_puntero_dir: " << input << " Output_puntero_val: " << *input<< endl;
}
SpatialConnection::~SpatialConnection(){

}
void SpatialConnection::Evaluate(){
	cout << "ID_input: " << input_node->GetId() << " ID_output: " << output_node->GetId();
	//double input = input_node->GetOuput();
	double input = *this->input;
	cout << " weight: " << weight << " Input: " << input << " Output: " << input*weight << endl;
	output_node->AddInputToNode(input*weight);
}
vector < double > SpatialConnection::GetInputCoordenates(){
	return input_node->GetCoordenates();
}
vector < double > SpatialConnection::GetOutputCoordenates(){
	return output_node->GetCoordenates();
}

#endif