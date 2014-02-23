#ifndef SUBSTRATE_H
#define SUBSTRATE_H

#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "SpatialNode.hpp"
using namespace std;

namespace ANN_USM{
	class Substrate
	{
		int coordenate_type;//cartesian 2D: 0, cartesian 3D: 1, polar 2D: 2, polar 3D: 3.
		int n_layers;//layers number
		vector < int > n_layer_nodes;//nodes number for each layer
		vector < vector < vector < int > > > nodes_info;//nodes type, input/output id and layer id for each node of each layer
		vector < vector < vector < double > > > nodes_coordenate;//nodes coordenates for each node of each layer
		vector < double * > inputs;//vector of inputs substrate
		vector < double * > outputs;//vector of outputs substrate
		vector < vector < SpatialNode * > > nodes;//vector of all nodes in the substrate
	public:
		Substrate(vector < double * > inputs, vector < double * > outputs);
		~Substrate();
		void SJsonDeserialize(char * substrate_info);//Extract all substrate information of char pointer
		void CreateNodes();//Inicialize overall nodes based of information extract with SJsonDeserialize function
		int GetCoordenateType();//Return the substrate coordenate type
		int GetLayersNumber();//Return the substrate layer number
		vector < int > GetLayerNodesNumber();//Return a vector of nodes number of each layer in the substrate
		SpatialNode * GetSpatialNode(int layer_num, int layer_node_num);//Return the indicated node
		void EvaluateSpatialNode(int layer_num, int layer_node_num);
		void ClearSpatialNodeInputs(int layer_num, int layer_node_num);
		double GetSpatialNodeOutput(int layer_num, int layer_node_num);
	};
}
#endif
