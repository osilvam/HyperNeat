#ifndef GENETIC_ENCODING_hPP
#define GENETIC_ENCODING_hPP


#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <strings.h>

#include "function.hpp"

using namespace std;

namespace ANN_USM
{
	enum gene_type
	{
		INPUT,
		HIDDEN,
		OUTPUT
	};

	//==================================================================
	//	Connection Gene
	//==================================================================

	/** \brief Class containing the connections' characteristics. */
	class connection_gene
	{
		public:

			void c_g(int innovation, int in, int out, double weight, bool enable);
			void kill();

			/**	Innovation number used to identify any connection ever created, making
			*	the mating process easier. */
			int innovation;

			/**	Node id from which the connection starts. */
			int in;

			/**	Node id in which the connection ends up. */
			int out;

			/**	This attribute, unlike _exist_, tells that the connection exists but is 
			*	not considered for the calculus. Subsequently it can be reactivated, 
			*	maintaining the same characteristics thanks to the innovation number. */
			bool enable;

			/**	This attribute, unlike _enable_, tells that the connection doesn't exist. 
			*	Subsequently it can be created. */
			bool exist; 

			/**	Connection's weight. */
			double weight;
			
	};

	//==================================================================
	//	Node Gene
	//==================================================================

	/** \brief Class containing the nodes' characteristics. */
	class node_gene
	{
		public:

			node_gene();

			void n_g(int node, int row, gene_type type, string function); // fill a node gene
			void kill();
			void increase_incoming_connection();
			void decrease_incoming_connection();
			void eval(double value);

			double get_final_result();

			/**	This tells if into the current node have arrived all the incoming connections. */
			bool is_ready();

			/**	Row in which the node is positioned.
			*	Rows are used to prevent _recurrent connections_ (backward connections). This feature 
			*	is not yet supported (and presumably will never be implemented because of the odd 
			*	behaviour it generates in the network). */
			int row;

			/**	Node's id */
			int node;

			/**	__(During creation)__ Number of node's total incoming connections. This is the total 
			*	number of connections that arrived to this node. */
			int incoming_connections;

			/**	__(During evaluation)__ Number of node's finished incoming connections. This is the 
			*	number of connections that has been sent from the previous nodes, which already have had all their 
			*	incoming connections ready. */
			int counter;

			/**	While the _counter_ variable doesn't reach the _incoming_connections_ value, the connections' values
			*	that arrived from the other (previous) nodes is summed to this variable. */
			double accumulative_result;

			/**	When the _counter_ variable finally reaches the _incoming_connections_ value, the _accumulative_result_
			*	variable is evaluated and stored in here. */
			double final_result;

			/**	If _false_ it means that the node has not been already created. Otherwise the contrary. */
			bool exist;

			/**	Specify the node's type, i.e. _INPUT_, _OUTPUT_ or _HIDDEN_*/
			gene_type type;

			/** Specify the node's activation function, i.e. _IDENTITY_, _GAUSSIAN_, _SIN_, etc ...*/
			Function * function;
	};

	//==================================================================
	//	Gene Encoding
	//==================================================================

	/** \brief Class containing the organisms' characteristics. */
	class Genetic_Encoding
	{
		public:

			void add_node(int node, int row, gene_type type, string function);
			void add_node(node_gene node);
			void add_connection(int innovation, int in, int out, double weight, bool enable);
			void add_connection(connection_gene conn);
			void change_weight(int innovation, double weight);
			void save(char path[]);
			void load(char path[]);
			void CJsonDeserialize(char buffer[]);
			void spread_final_result(int node, double value);

			string JSON();

			vector<connection_gene> get_outgoing_connections(int node);
			
			vector<double> eval(vector <double> inputs);

			/**	List of connections. */
			vector<connection_gene> Lconnection_genes;

			/**	List of nodes. */
			vector<node_gene> Lnode_genes;

			/**	Vector of the nodes' id which are inputs. */
			vector<int> input_nodes;			

			/**	Vector of the nodes' id which are outputs. */
			vector<int> output_nodes;	

			/**	Niche's id in which the current organism belong to. */
			int niche;

			/**	Fitness value of the current organism. */
			double fitness;
	};
}

ostream & operator<<(ostream & o, ANN_USM::Genetic_Encoding & encoding);

#endif