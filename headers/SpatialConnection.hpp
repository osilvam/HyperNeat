#ifndef SPATIALCONNECTION_H
#define SPATIALCONNECTION_H

#include "SpatialNode.hpp"
#include <vector>
#include <iostream>
using namespace std;
/**
 * \namespace ANN_USM
 * \brief Dedicated to artificial intelligence development in Santa María University.
 */
namespace ANN_USM{
	/**
	 * \class SpatialConnection
	 * \brief The class SpatialConnection is used to create connections among overall nodes in a HyperNeat Substrate.
	 */
	class SpatialConnection
	{	
		SpatialNode * input_node;/**< Pointer to input node */
		SpatialNode * output_node;/**< Pointer to output node */
		double weight;/**< weight value. Weight associated to connection */
		double * input;/**< Pointer from input connection */
		double * output;/**< Pointer to output connection */

	public:	
		/**
		 * \brief Constructor
		 * \param input_node Pointer to input node
		 * \param output_node Pointer to output node
		 * \param weight Weight associated to connection
		 */
		SpatialConnection(SpatialNode * input_node, SpatialNode * output_node, double weight);
		/**
		 * \brief Destructor
		 */
		~SpatialConnection();
		/**
		 * \brief Evalue the spatial connection output
		 */
		void Evaluate();
		/**
		 * \brief Get the input node cordenates
		 * \return Vector of coordenates
		 */
		vector < double > GetInputCoordenates();
		/**
		 * \brief Get the output node coordenates
		 * \return Vector of coordenates
		 */
		vector < double > GetOutputCoordenates();

	};
}
#endif