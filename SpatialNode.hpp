#ifndef SPATIALNODE_H
#define SPATIALNODE_H

#include <vector>
#include <iostream>
using namespace std;

namespace ANN_USM{
	class SpatialNode
	{	
		int layer_id;//node layer id in the substrate
		int node_type;//input: 0, hidden: 1, output: 2.
		vector < double > coordenates;//node cordenates
		int n_inputs;//inputs number
		vector < double * > inputs;//vector of inputs from other nodes
		double * input;//substrate input only if the node is in input mode
		double * output;//output node
	public:

		SpatialNode(int node_type, int layer_id, vector < double > coordenates);
		~SpatialNode();
		void SetInputToInputNode(double * input);//Assign input to input type node
		void SetOutputToOutputNode(double * output);//Assign output to output type node
		void AddInputToNode(double * input);//Add input to node with spatial connection
		void AddOutputToNode(double * output);//Assign input to spatial connection from output node		
		void OutputCalcule();//Calcule of node output value
		vector < double > GetCoordenates();//Return the node cordenates
		int GetNodeType();//Return the node type
		int GetLayerNodeId();//Return the node layer id
	};
}
#endif
