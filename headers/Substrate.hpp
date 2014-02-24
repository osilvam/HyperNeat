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

		int n_layouts;//layouts number
		vector < int > coordenate_type;//cartesian 2D: 0, cartesian 3D: 1, polar 2D: 2, polar 3D: 3. of each layout
		vector < int > n_layers;//layers number of each layout
		vector < vector < int > > n_layer_nodes;//nodes number for each layer of each layout
		vector < vector < vector < vector < int > > > > nodes_info;//nodes type, input/output id and layer id for each node of each layer of each layout
		vector < vector < vector < vector < double > > > > nodes_coordenate;//nodes coordenates for each node of each layer of each layout
		vector < double * > inputs;//vector of inputs substrate
		vector < double * > outputs;//vector of outputs substrate
		vector < vector < vector < SpatialNode * > > > nodes;//vector of nodes in each layer of each layout in the substrate
		
	public:
		Substrate(vector < double * > inputs, vector < double * > outputs);
		Substrate();
		~Substrate();
		void SJsonDeserialize(char * substrate_info);//Extract all substrate information of char pointer
		void CreateNodes();//Inicialize overall nodes based of information extract with SJsonDeserialize function
		int GetLayoutNumber();
		int GetCoordenateType(int layout_num);//Return the substrate coordenate type
		int GetLayersNumber(int layout_num);//Return the substrate layer number
		vector < int > GetLayerNodesNumber(int layout_num);//Return a vector of nodes number of each layer in the substrate
		SpatialNode * GetSpatialNode(int layout_num, int layer_num, int layer_node_num);//Return the indicated node
		void EvaluateSpatialNode(int layout_num);
		void EvaluateSpatialNode(int layout_num, int layer_num);
		void ClearSpatialNodeInputs(int layout_num);
		void ClearSpatialNodeInputs(int layout_num, int layer_num);
		double GetSpatialNodeOutput(int layout_num, int layer_num, int layer_node_num);
		double GetSpatialNodeId(int layout_num, int layer_num, int layer_node_num);
		void PrintInputs();
		
	};
}
#endif
