#ifndef SPATIALCONNECTION_H
#define SPATIALCONNECTION_H

#include "SpatialNode.hpp"
#include <vector>
#include <iostream>
using namespace std;

namespace ANN_USM{
	class SpatialConnection
	{	
		SpatialNode * input_node;//input node
		SpatialNode * output_node;//output node
		double weight;//weight of the connection
		//double * input;//pointer from input connection
		//double * output;//pointer to output connection

	public:	

		SpatialConnection(SpatialNode * input_node, SpatialNode * output_node, double weight);
		~SpatialConnection();
		void Evaluate();//Evalue the spatial connection output
		vector < double > GetInputCoordenates();//Get the input node cordenates
		vector < double > GetOutputCoordenates();//Get the output node coordenates

	};
}
#endif