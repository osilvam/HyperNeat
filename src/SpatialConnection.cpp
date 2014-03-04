#ifndef SPATIALCONNECTION_CPP
#define SPATIALCONNECTION_CPP

#include "SpatialConnection.hpp"
using namespace ANN_USM;

SpatialConnection::SpatialConnection(SpatialNode * input_node, SpatialNode * output_node, double weight){
	if(output_node->GetNodeType() == 0){
		cout << "can not connect to a node of type input" << endl;
		return;
	}
	if (input_node->GetSheetNodeId() > output_node->GetSheetNodeId()){			
		cout << "can not make a recurrent connection" << endl;
		return;
	}
	this->input_node = input_node;
	this->output_node = output_node;
	this->weight = weight;
	input = new double;
	output = new double;
	input = input_node->AddOutputToNode();
	output_node->AddInputToNode(output);

	cout << "Input_ID: " << input_node->GetId() << " Output_ID: " << output_node->GetId();
	cout << " Weight: " << weight << endl; 

}
SpatialConnection::~SpatialConnection(){

}
void SpatialConnection::Evaluate(){
	*output = (*input)*weight;
	//cout << "Input_ID: " << input_node->GetId() << " Output_ID: " << output_node->GetId();
	//cout << " Weight: " << weight << endl; 
	//cout << " INPUT: " << input << " OUTPUT: " << output << endl;
}
vector < double > SpatialConnection::GetInputCoordenates(){
	return input_node->GetCoordenates();
}
vector < double > SpatialConnection::GetOutputCoordenates(){
	return output_node->GetCoordenates();
}

#endif