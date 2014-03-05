#ifndef HYPERNEAT_H
#define HYPERNEAT_H

#include "Substrate.hpp"
//#include "ConnectiveCPPN.hpp"
#include "CPPNInputs.hpp"
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
using namespace std;
/**
 * \namespace ANN_USM
 * \brief Dedicated to artificial intelligence development in Santa María University.
 */
namespace ANN_USM{
	/**
	 * \class HyperNeat
	 * \brief The class HyperNeat is used to implement a neuroevolution method called HyperNeat
	 */
	class HyperNeat
	{
		int n_AditionalCPPNInputs;/**< Number of aditional cppn inputs */	
		vector < CPPNInputs > AditionalCPPNInputs;/**< Vector of aditional cppn inputs */
		int connection_threshold;/**< Threshold that determine the creation for an connection */
		vector < double > CppnInputs;/**< Vector of cppn input values */
		int n_connections;/**< Number of connections after evaluate connections with cppn-neat */
		//ConnectiveCPPN * cppn;
		Substrate * substrate;/**< Vector of overall HyperNeat substrates */
	public:
		/**
		 * \brief Constructor with parameters
		 * \param inputs Input vector
		 * \param outputs Output vector
		 * \param hyperneat_info Json string
		 */
		HyperNeat(vector < double * > inputs, vector < double * > outputs, string hyperneat_info);
		/**
		 * \brief Destructor
		 */
		~HyperNeat();
		/**
		 * \brief Extract all HyperNeat information of json string
		 * \param hyperneat_info json string
		 */
		void HJsonDeserialize(string hyperneat_info);
		/**
		 * \brief Create all substrate connections according to cppn-neat result
		 */
		void CreateSubstrateConnections();
		/**
		 * \brief Allows to obtain the final HyperNeat outputs
		 */
		void EvaluateSubstrateConnections();
		/**
		 * \brief Set CPPN-NEAT fitness of last interation
		 * \param fitness Fitnnes value to set
		 */
		void HyperNeatFitness(double fitness);
		/**
		 * \brief Allows evolve cppn_neat
		 */
		void HyperNeatEvolve();		
		/**
		 * \brief Allows obtain all final functions of every output node
		 * \return vector with all final output functions
		 */
		vector < string > GetHyperNeatOutputFunctions();
	};
}
#endif