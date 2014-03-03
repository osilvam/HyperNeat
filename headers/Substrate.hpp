#ifndef SUBSTRATE_H
#define SUBSTRATE_H

#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "SpatialNode.hpp"
using namespace std;
/**
 * \namespace ANN_USM
 * \brief Dedicated to artificial intelligence development in Santa María University.
 */
namespace ANN_USM{
	/**
	 * \class Substrate
	 * \brief The class Substrate is used to create HyperNeat Substrate
	 */
	class Substrate
	{	
		int n_layouts;/**< Layouts number */
		vector < int > coordenate_type;/**< Coordenate type vector. Cartesian 2D: 0, cartesian 3D: 1, polar 2D: 2, polar 3D: 3. of each layout */
		vector < int > n_layers;/**< Layers number of each layout */
		vector < vector < int > > n_layer_nodes;/**< Nodes number for each layer of each layout */
		vector < vector < vector < vector < int > > > > nodes_info;/**< Nodes type, input/output id and layer id for each node of each layer of each layout */
		vector < vector < vector < vector < double > > > > nodes_coordenate;/**< Nodes coordenates for each node of each layer of each layout */
		vector < double * > inputs;/**< Vector of inputs substrate */
		vector < double * > outputs;/**< Vector of outputs substrate */
		vector < vector < SpatialNode * > > nodes;/**< Vector of nodes in each layer of each layout in the substrate */
		
	public:
		/**
		 * \brief Constructor with parameters
		 * \param inputs Input vector
		 * \param outputs Output vector
		 */
		Substrate(vector < double * > inputs, vector < double * > outputs);
		/**
		 * \brief Void Constructor
		 */
		Substrate();
		/**
		 * \brief Destructor
		 */
		~Substrate();
		/**
		 * \brief Extract all Substrate information of char pointer
		 * \param substrate_info char pointer of json string
		 */
		void SJsonDeserialize(char * substrate_info);
		/**
		 * \brief Inicialize overall nodes based of information extract with SJsonDeserialize function
		 */
		void CreateNodes();
		/**
		 * \brief Get Substrate layout number
		 */
		int GetLayoutNumber();
		/**
		 * \brief Get layout coordenate type
		 * \param layout_num Layout number
		 * \return Substrate coordenate type
		 */
		int GetCoordenateType(int layout_num);
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
		
	};
}
#endif
