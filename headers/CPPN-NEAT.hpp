
#ifndef CPPN_NEAT_HPP
#define CPPN_NEAT_HPP

#include <cmath>
#include <unistd.h>

#include "genetic_encoding.hpp"

using namespace std;

/** \brief Main _namespace_ used to differentiate from other _ANN_ applications. */
namespace ANN_USM
{
	//===================================================================================
	//	Niche
	//===================================================================================

	/** \brief Class containing the niches' characteristics. */
	class Niche
	{
		public:

			int niche_champion_position;
			int amount_of_offspring;

			bool exist;

			double total_fitness;
			
			vector<int> organism_position;
	};

	//===================================================================================
	//	Population
	//===================================================================================

	/** \brief Class containing the populations' characteristics. */
	class Population
	{
		public:

			Population();

			void CJsonDeserialize(char pch[]);

			void save_all(char path[]);
			void save(char path[]);
			void epoch();
			void spatiation();
			void SetFitness(double fitness, int organism_id);

			int obtain_historical_node(int initial_in, int initial_out);
			int obtain_historical_innovation(int in, int out);
			int obtain_row(int node, int node_initial_in, int node_initial_out);

			double compatibility(Genetic_Encoding orgm1, Genetic_Encoding orgm2); // Distance between two ANNs
			
			vector<double> CalculeWeight(vector<double> cppn_inputs, int organism_id);

			Genetic_Encoding mutation_node(Genetic_Encoding organism);
			Genetic_Encoding mutation_connection(Genetic_Encoding organism);
			Genetic_Encoding mutation_change_weight(Genetic_Encoding organism);
			Genetic_Encoding put_randoms_weight(Genetic_Encoding organism);
			Genetic_Encoding crossover(Genetic_Encoding orgm1, Genetic_Encoding orgm2);

			int lenght;
			int total_niches;
			int last_niche_id;
			int last_innovation;
			int last_node;
			int last_row;

			double fitness_champion;

			vector<Genetic_Encoding> organisms;
			vector<Genetic_Encoding> prev_organisms;

			vector <Niche> current_niches;
			vector <Niche> prev_niches;

			Genetic_Encoding * best_organism;
			Genetic_Encoding champion;

			vector< vector<int> > historical_nodes;

			/**	Connections' historical innovation number between two nodes.
			*
			*	-	If both nodes were connected at any space-time in the past generations (or even in the 
			*		current one) then they will have the very same innovation number as they had before.
			*
			*	-	If they weren't connected at all, i.e. a value of -1, then the connection will obtain a 
			*		new innovation number.
			*
			*	Here the main vector's indexes represent all of the nodes (where the connections are originated).
			*	In the same way, each vector's indexes within the main vector represent all the nodes as well 
			*	(where the connections end). 
			*	
			*	In oder words, it becomes the matrix representation of a directed graph. */
			vector< vector<int> > historical_innovation;

			vector<int> historical_row;

			/**	*/
			vector<int> row_orderer_list;
	};
}

ostream & operator<<(ostream & o, ANN_USM::Population & pop);

#endif