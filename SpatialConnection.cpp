#ifndef SPATIALCONNECTION_CPP
#define SPATIALCONNECTION_CPP

#include "SpatialConnection.hpp"
using namespace ANN_USM;

SpatialConnection::SpatialConnection(SpatialNode input_node, SpatialNode output_node, double weight){
	if(output_node.GetNodeType() == 0){
		cout << "can not connect to a node of type input" << endl;
		return;
	}
	if (input_node.GetLayerNodeId() >= output_node.GetLayerNodeId()){			
		cout << "can not make a recurrent connection" << endl;
		return;
	}
	this->input_node = input_node;
	this->output_node = output_node;
	this->weight = weight;
	input_node.AddOutputToNode(this->input);
	output_node.AddInputToNode(this->output);
	*output = (*input)*weight;
}
SpatialConnection::~SpatialConnection(){

}
void SpatialConnection::SpatialConnection::Evaluate(){
	*output = (*input)*weight;
}
vector < double > SpatialConnection::GetInputCoordenates(){
	return input_node.GetCoordenates();
}
vector < double > SpatialConnection::GetOutputCoordenates(){
	return output_node.GetCoordenates();
}

#endif