#ifndef SPATIALNODE_H
#define SPATIALNODE_H

#include <vector>
#include <iostream>
#include "UserFunctions.hpp"
using namespace std;
/**
 * \namespace ANN_USM
 * \brief Dedicated to artificial intelligence development in Santa María University.
 */
namespace ANN_USM{
	/**
	 * \class SpatialNode
	 * \brief The class SpatialNode is used to create nodes in a HyperNeat Substrate.
	 */
	class SpatialNode
	{	
		int id;/**< node id value */
		int sheet_id;/**< node sheet id value */
		int node_type;/**< node type value. Input: 0, Hidden: 1, Output: 2 */
		vector < double > coordenates;/**< coordenate node vector */
		int n_inputs;/**< inputs number */
		vector < double * > inputs;/**< vector of input pointer from other nodes */
		double * input;/**< substrate input pointer. Only if the node is in input mode */
		double * output;/**< output node pointer */
	public:
		/**
		 * \brief Constructor with parameters
		 * \param id Id value
		 * \param node_type Node type value
		 * \param sheet_id Sheet id value
		 * \param coordenates Coordenate node vector
		 */
		SpatialNode(int id, int node_type, int sheet_id, vector < double > coordenates);
		/**
		 * \brief Void constructor
		 */
		SpatialNode();
		/**
		 * \brief Desstructor
		 */
		~SpatialNode();
		/**
		 * \brief Assign input to input type node
		 * \param input Input pointer
		 */
		void SetInputToInputNode(double * input);
		/**
		 * \brief Assign output to output type node
		 * \param output Output pointer
		 */
		void SetOutputToOutputNode(double * output);
		/**
		 * \brief Add input to node with spatial connection
		 * \param input Input pointer
		 */
		void AddInputToNode(double * input);
		/**
		 * \brief Add output to node with spatial connection
		 * \return Output pointer
		 */
		double * AddOutputToNode();		
		/**
		 * \brief Calcule of node output value
		 */
		void OutputCalcule();
		/**
		 * \brief Get coordenates node
		 * \return Coordenate node vector
		 */
		vector < double > GetCoordenates();
		/**
		 * \brief Get node id.
		 * \return node id value. Input: 0, Hidden: 1, Output: 2
		 */
		int GetId();		
		/**
		 * \brief Get node type
		 * \return node type value
		 */
		int GetNodeType();
		/**
		 * \brief Get sheet node id
		 * \return Sheet node id value
		 */
		int GetSheetNodeId();
		/**
		 * \brief Get node output
		 * \return node output value
		 */
		double GetOuput();
		/**
		 * \brief Clear inputs node vector
		 */
		void ClearInputs();
	};
}
#endif
