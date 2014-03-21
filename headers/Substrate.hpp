#ifndef SUBSTRATE_H
#define SUBSTRATE_H

#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

#include "SpatialNode.hpp"

using namespace std;

namespace ANN_USM
{
	class Substrate
	{	
		// Number of layouts in the substrate
		int n_layouts;

		// Coordinate type vector of each layout
		//   Cartesian 2D: 0
		//   Cartesian 3D: 1
		//   Polar 2D: 2
		//   Polar 3D: 3
		vector < int > coordinate_type; 

		// Number of layers within each layout
		vector < int > n_layers; 		

		// Number of nodes within each layer (for all layout)
		vector < vector < int > > n_layer_nodes; 

		// Type, input / output id and layer id of each node (for all layer within every layout)
		vector < vector < vector < vector < int > > > > nodes_info;

		// Coordinates of each node (for all layer within every layout)
		vector < vector < vector < vector < double > > > > nodes_coordinate;
		
		// Vector of nodes in each layer of each layout in the substrate
		vector < vector < SpatialNode * > > nodes; 
		
	public:

		vector< double * > inputs;/**< Vector of inputs pointer of Substrate */
		vector< double * > outputs;/**< Vector of outputs pointer of Substrate */

		Substrate(vector < double * > inputs, vector < double * > outputs);
		Substrate();
		~Substrate();
		/**
		 * \brief Extract all Substrate information of char pointer
		 * \param substrate_info char pointer of json string
		 */
		char * SJsonDeserialize(char * substrate_info);
		/**
		 * \brief Inicialize overall nodes based of information extract with SJsonDeserialize function
		 */
		void CreateNodes();
		/**
		 * \brief Get Substrate layout number
		 */
		int GetLayoutNumber();
		/**
		 * \brief Get layout coordinate type
		 * \param layout_num Layout number
		 * \return Substrate coordinate type
		 */
		int GetCoordinateType(int layout_num);
		/**
		 * \brief Get layout layer number
		 * \param layout_num Layout number
		 * \return Substrate layer number
		 */
		int GetLayersNumber(int layout_num);
		/**
		 * \brief Get layer node number of specific layout
		 * \param layout_num Layout number
		 * \param layer_num Layer number
		 * \return Vector of nodes number of each layer in the Substrate
		 */
		int GetLayerNodesNumber(int layout_num, int layer_num);
		/**
		 * \brief Get spatial node of specific layout and layer
		 * \param layout_num Layout number
		 * \param layer_num Layer number
		 * \param layer_node_num Layer node number
		 * \return Indicated spatial node pointer
		 */
		SpatialNode * GetSpatialNode(int layout_num, int layer_num, int layer_node_num);
		/**
		 * \brief Evaluate spatial node outputs of specific layout and layer
		 * \param layout_num Layout number
		 * \param layer_num Layer number
		 */
		void EvaluateSpatialNode(int layout_num, int layer_num);
		/**
		 * \brief Clear input vector of spatial node in specific layout and layer
		 * \param layout_num Layout number
		 * \param layer_num Layer number
		 */
		void ClearSpatialNodeInputs(int layout_num, int layer_num);
		/**
		 * \brief Get spatial node output of specific layout and layer
		 * \param layout_num Layoput number
		 * \param layer_num Layer number
		 * \param layer_node_num Layer node number
		 * \return Output value of specific spatial node
		 */
		double GetSpatialNodeOutput(int layout_num, int layer_num, int layer_node_num);
		/**
		 * \brief Get spatial node id of specific layout and layer
		 * \param layout_num Layoput number
		 * \param layer_num Layer number
		 * \param layer_node_num Layer node number
		 * \return Id of specific spatial node
		 */
		double GetSpatialNodeId(int layout_num, int layer_num, int layer_node_num);
		/**
		 * \brief Allows obtain all final functions of every output node
		 * \return vector with all final output functions
		 */
		vector < string > GetSubstrateOutputFunctions(string plataform);
		
	};
}
#endif
