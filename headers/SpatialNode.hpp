#ifndef SPATIALNODE_H
#define SPATIALNODE_H

#include <vector>
#include <iostream>
using namespace std;

namespace ANN_USM{
	class SpatialNode
	{	
		int id;//TEMPORAL
		int sheet_id;//node layer id in the layout
		int node_type;//input: 0, hidden: 1, output: 2.
		vector < double > coordenates;//node cordenates
		int n_inputs;//inputs number
		vector < double * > inputs;//vector of inputs from other nodes
		double * input;//substrate input only if the node is in input mode
		double * output;//output node
	public:

		SpatialNode(int id, int node_type, int sheet_id, vector < double > coordenates);
		SpatialNode();
		~SpatialNode();
		void SetInputToInputNode(double * input);//Assign input to input type node
		void SetOutputToOutputNode(double * output);//Assign output to output type node
		void AddInputToNode(double * input);//Add input to node with spatial connection
		double * AddOutputToNode();//Add output to node with spatial connection		
		void OutputCalcule();//Calcule of node output value
		vector < double > GetCoordenates();//Return the node cordenates
		int GetId();		
		int GetNodeType();//Return the node type
		int GetSheetNodeId();//Return the node layer id
		double GetOuput();
		void ClearInputs();
	};
}
#endif
