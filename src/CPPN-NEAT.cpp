
#ifndef CPPN_NEAT_CPP
#define CPPN_NEAT_CPP

#include "CPPN-NEAT.hpp"
#include "UserFunctions.hpp"

using namespace ANN_USM;

//===================================================================================
//	Population
//===================================================================================

Population::Population()
{
	// VER BIEN ESTO
	// DEPENDE MUCHO DE LO QUE SUBA EL USUARIO
	//========================================
	row_orderer_list.push_back(0);
	row_orderer_list.push_back(1);
	last_row = 1;
	last_innovation = -1;
	//========================================

	fitness_champion = 0;
	
	lenght = POPULATION_MAX;

	Niche niche_temp;
	niche_temp.organism_position.push_back(0);
	niche_temp.exist = true;
	niche_temp.niche_champion_position = 0;
	current_niches.push_back(niche_temp);	
}

void Population::CJsonDeserialize(char pch[])
{
	Genetic_Encoding organism;

	organism.CJsonDeserialize(pch);

	organism.niche = 0;

	champion = organism;

	for (int i = 0; i < (int)organism.Lconnection_genes.size(); ++i)
		obtain_historical_innovation(organism.Lconnection_genes.at(i).in, organism.Lconnection_genes.at(i).out); 

	last_node = (int)organism.Lnode_genes.size() - 1;

	for (int i = 0; i < POPULATION_MAX; ++i)
		organisms.push_back(mutation_node(put_randoms_weight(organism))); 

	prev_organisms.push_back(organism);

	spatiation();
}

vector<double> Population::CalculeWeight(vector<double> cppn_inputs, int organism_id)
{
	if(organism_id == -1) return champion.eval(cppn_inputs);
	
	return organisms.at(organism_id).eval(cppn_inputs);
}

Genetic_Encoding Population::put_randoms_weight(Genetic_Encoding organism)
{
	for (int i = 0; i < (int)organism.Lconnection_genes.size(); ++i)
		organism.Lconnection_genes.at(i).weight = 2.0 * (rand() % 10000) / 10000.0 - 1.0;

	return organism;
}

Genetic_Encoding Population::mutation_change_weight(Genetic_Encoding organism)
{
	int connection_to_mutate = round(rand() % int(organism.Lconnection_genes.size()));

	double delta = 2 * (rand() % 10000) / 10000.0 - 1;

	organism.Lconnection_genes.at(connection_to_mutate).weight = delta * 0.3 + 0.7 * organism.Lconnection_genes.at(connection_to_mutate).weight;

	return organism;
}

Genetic_Encoding Population::mutation_node(Genetic_Encoding organism)
{
	int number_of_connections = organism.Lconnection_genes.size();
	int connection_to_mutate;
	int innov1;
	int innov2;
	int node;
	int row;

	for(int i = 0; i <= 50; i++)
	{
		if(i == 50)
		{
			cerr << "In function Mutation_node:: in 50 attempts not found an mutation option";
			return organism;
		}

		// Searches for an existing connection
		while(true)
		{
			connection_to_mutate = round(rand() % number_of_connections);
			if(organism.Lconnection_genes.at(connection_to_mutate).exist) break;
		}
		
		// Obtain it's historical node number
		node = obtain_historical_node(organism.Lconnection_genes.at(connection_to_mutate).in, organism.Lconnection_genes.at(connection_to_mutate).out);

		if(!(node < (int)organism.Lnode_genes.size()) || !organism.Lnode_genes.at(node).exist) break;		
	}

	row = obtain_row(node, organism.Lnode_genes[organism.Lconnection_genes[connection_to_mutate].in].row, organism.Lnode_genes[ organism.Lconnection_genes[connection_to_mutate].out ].row);

	Function function;
	organism.add_node(node, row , HIDDEN, function.get_name((int)round(rand() % FUNCTION_NUM)));
	
	// disabling old connection.
	organism.Lconnection_genes[connection_to_mutate].enable = 0; 
	organism.Lnode_genes.at(organism.Lconnection_genes[connection_to_mutate].out).decrease_incoming_connection();

	innov1 = obtain_historical_innovation(organism.Lconnection_genes[connection_to_mutate].in, node);
	innov2 = obtain_historical_innovation(node, organism.Lconnection_genes[connection_to_mutate].out);
	
	organism.add_connection(innov1, organism.Lconnection_genes[connection_to_mutate].in, node, 1.0, true);
	organism.add_connection(innov2, node, organism.Lconnection_genes[connection_to_mutate].out, organism.Lconnection_genes[connection_to_mutate].weight, true);
	
	return organism;
}

// Obtain the row number if it can be fixed within one in concrete, or creates one new if there is no 
// an available one.
int Population::obtain_row(int node, int row_node_initial_in, int row_node_initial_out)
{	
	int row_position_in = -1;
	int row_position_out = -1;

	bool flag_in = false;
	bool flag_out = false;

	for(int i = 0; i < (int)row_orderer_list.size(); ++i)
	{
		if(!flag_in && row_node_initial_in == row_orderer_list.at(i)) 
		{  
			row_position_in = i; 
			flag_in = true; 
		}

		if(!flag_out && row_node_initial_out == row_orderer_list.at(i))
		{  
			row_position_out = i; 
			flag_out = true;
		}

		if(flag_in && flag_out) break;
	}
	
	if(row_position_in == -1 || row_position_out == -1 )
	{
		cerr << row_node_initial_in << "\t" << row_position_in << "\t" << row_node_initial_out << "\t" << row_position_out << "\t" << row_orderer_list.size() << endl << "Error:: Function obtain_row :: Row_node_in or row_node_out does not exist .\n"; 
		exit(1);
	}

	if( row_position_in > row_position_out)
	{
		cerr << row_node_initial_in << "\t" << row_position_in << "\t" << row_node_initial_out << "\t" << row_position_out << "\t" << row_orderer_list.size() << endl; 
		cerr << "Error:: Function obtain_row :: ------------------------.\n"; 
		exit(1);
	}

	while((int)historical_row.size() - 1 < node)
		historical_row.push_back(-1);

	if(historical_row[node] >= 0) 
		return historical_row[node];
	
	if(row_position_in == row_position_out)
		return row_node_initial_in;
	else
	{
		if(row_position_out - row_position_in == 1)
		{
			historical_row.at(node) = ++last_row;
			row_orderer_list.insert(row_orderer_list.begin() + row_position_in + 1, last_row);
		}
		else historical_row.at(node) = row_orderer_list[row_position_in + 1];

		return historical_row.at(node);
	}	
}

int Population::obtain_historical_node(int in, int out)
{
	// If the inner node of the connection is greater than the vector size,
	// fill with empty vectors until reach the desired node.
	while((int)historical_nodes.size() - 1 < in)
		historical_nodes.push_back(vector<int> ());

	// If the outgoing node of the connection is greater than the vector size
	// at the inner position, fill with -1 until reach the desired node.
	while((int)historical_nodes.at(in).size() - 1 < out)
		historical_nodes.at(in).push_back(-1);

	// If the pair of nodes weren't connected in the past then obtain a new
	// innovation number.
	// If they were connected at all then skip the if statement and return the corresponding innovation number.
	if(historical_nodes.at(in).at(out) < 0)
		historical_nodes.at(in).at(out) = ++last_node;

	return historical_nodes.at(in).at(out);
}

int Population::obtain_historical_innovation(int in, int out)
{
	// If the inner node of the connection is greater than the vector size,
	// fill with empty vectors until reach the desired node.
	while((int)historical_innovation.size() - 1 < in)
		historical_innovation.push_back(vector<int> ());

	// If the outgoing node of the connection is greater than the vector size
	// at the inner position, fill with -1 until reach the desired node.
	while((int)historical_innovation.at(in).size() - 1 < out)
		historical_innovation.at(in).push_back(-1);

	// If the pair of nodes weren't connected in the past then obtain a new
	// innovation number.
	// If they were connected at all then skip the if statement and return the corresponding innovation number.
	if(historical_innovation.at(in).at(out) < 0)
		historical_innovation.at(in).at(out) = ++last_innovation;

	return historical_innovation.at(in).at(out);
}

Genetic_Encoding Population::mutation_connection(Genetic_Encoding organism)
{
	int node_in;
	int node_out;
	int innovation;
	int number_of_nodes = (int)organism.Lnode_genes.size();

	// Try 50 times. If nothing happen then return the same organism
	for(int j = 0; j < 50; j++)
	{
		node_in = round(rand() % number_of_nodes);
		node_out = round(rand() % number_of_nodes);

		int row_position_in = -1;
		int row_position_out = -1;
		bool flag_in = false;
		bool flag_out = false;
		
		for (int i = 0; i < (int)row_orderer_list.size(); ++i)
		{
			if(organism.Lnode_genes[node_in].row == row_orderer_list.at(i))
			{   
				row_position_in = i; 
				flag_in = true;
			}

			if(organism.Lnode_genes[node_out].row == row_orderer_list.at(i))
			{ 	
				row_position_out = i; 
				flag_out = true;
			}

			if(flag_in && flag_out) break;
		}

		if((organism.Lnode_genes[node_out].exist && organism.Lnode_genes[node_in].exist) && ( row_position_in <  row_position_out))
		{
			innovation = obtain_historical_innovation(node_in, node_out);

			if(((int)organism.Lconnection_genes.size() - 1 < innovation) || !organism.Lconnection_genes[innovation].exist)
				organism.add_connection(innovation, node_in, node_out, 2*(rand()%10000)/10000.0 - 1.0, true);

			break;
		}
	}
	
	return organism;
}

ostream & operator<<(ostream & o, ANN_USM::Population & pop) 
{ 
	for (int i = 0; i < pop.lenght; ++i)
		o << pop.organisms.at(i) << endl;

	return o;
}

void Population::save_all(char path[])
{
	ofstream file;
	file.open (path);
	file << *this;
	file.close();
}

void Population::save(char path[])
{
	this->champion.save(path);
}

// Measure the distance between two organism. 
double Population::compatibility(Genetic_Encoding orgm_1, Genetic_Encoding orgm_2)
{
	int E = 0;
	int D = 0;
	int size_1 = orgm_1.Lconnection_genes.size();
	int size_2 = orgm_2.Lconnection_genes.size();
	int limit_sup;
	int limit_inf;

	double W = 0.;

	// Get the connections' length
	limit_sup = (size_1 > size_2) ? size_2 : size_1;
	limit_inf = (size_1 < size_2) ? size_1 : size_2;

	// Check the connections until the inferior limit
	// "Disjoint"
	for (int i = 0; i < limit_inf; ++i)
	{
		// If both connections at the same position exist
		if(orgm_1.Lconnection_genes.at(i).exist && orgm_2.Lconnection_genes.at(i).exist)
			W += abs(orgm_1.Lconnection_genes.at(i).weight - orgm_2.Lconnection_genes.at(i).weight);

		// If only one of the connections exists
		else if(orgm_1.Lconnection_genes.at(i).exist || orgm_2.Lconnection_genes.at(i).exist)
			D++;
	}
		
	// Continue checking from the inferior limit to the superior limit
	// "Excess"
	for (int i = limit_inf; i < limit_sup; ++i)
		if((size_1 < size_2) ? orgm_2.Lconnection_genes.at(i).exist : orgm_1.Lconnection_genes.at(i).exist) 
			E++;

	// Return the distance
	return DISTANCE_CONST_1 * E / DISTANCE_CONST_4 + DISTANCE_CONST_2 * D / DISTANCE_CONST_4 + DISTANCE_CONST_3 * W;
}

Genetic_Encoding Population::crossover(Genetic_Encoding orgm_1, Genetic_Encoding orgm_2)
{
	Genetic_Encoding orgm_resutl;

	int conn_size_1 = orgm_1.Lconnection_genes.size();
	int conn_size_2 = orgm_2.Lconnection_genes.size();
	int node_size_1 = orgm_1.Lnode_genes.size();
	int node_size_2 = orgm_2.Lnode_genes.size();

	int conn_limit_sup;
	int conn_limit_inf;
	int node_limit_sup;
	int node_limit_inf;

	bool connection_1_is_larger, node_1_is_larger;

	if(conn_size_1 > conn_size_2) 
	{		
		conn_limit_sup = conn_size_1;		
		conn_limit_inf = conn_size_2;		
		connection_1_is_larger = true;
	}
	else
	{		
		conn_limit_sup = conn_size_2;		
		conn_limit_inf = conn_size_1;	
		connection_1_is_larger = false;	
	}

	if(node_size_1 > node_size_2) 
	{		
		node_limit_sup = node_size_1;		
		node_limit_inf = node_size_2;		
		node_1_is_larger = true;	
	}
	else
	{			
		node_limit_sup = node_size_2;		
		node_limit_inf = node_size_1;		
		node_1_is_larger = false;	
	}

	for(int  i = 0; i < node_limit_sup; i++)
	{
		if (i >= node_limit_inf)
		{
			if(node_1_is_larger)
			{
				if(orgm_1.Lnode_genes.at(i).exist == 1)
					orgm_resutl.add_node(orgm_1.Lnode_genes.at(i));
			}
			else
			{
				if(orgm_2.Lnode_genes.at(i).exist == 1)
					orgm_resutl.add_node(orgm_2.Lnode_genes.at(i));
			}
		}
		else
		{
			// If both nodes exist
			if( orgm_1.Lnode_genes.at(i).exist && orgm_2.Lnode_genes.at(i).exist)
				// 50% of chance of being in the new genome
				(rand()%10 >= 5) ? orgm_resutl.add_node(orgm_1.Lnode_genes.at(i)) : orgm_resutl.add_node(orgm_2.Lnode_genes.at(i));

			// If only one of them exists
			else if ( orgm_1.Lnode_genes.at(i).exist || orgm_2.Lnode_genes.at(i).exist)
				(orgm_1.Lnode_genes.at(i).exist) ? orgm_resutl.add_node(orgm_1.Lnode_genes.at(i)) : orgm_resutl.add_node(orgm_2.Lnode_genes.at(i));
		}
	}
	
	// WHAT IF A NODE IS NOT IN THE NEW ONE; BUT ITS CONNECTION IS
	//=============================================================
	for(int i = 0; i < conn_limit_sup; i++)
	{
		if (i >= conn_limit_inf)
		{
			if(connection_1_is_larger)
			{
				if(orgm_1.Lconnection_genes.at(i).exist==1)
					orgm_resutl.add_connection(orgm_1.Lconnection_genes.at(i));
			}
			else
			{
				if(orgm_2.Lconnection_genes.at(i).exist==1)
					orgm_resutl.add_connection(orgm_2.Lconnection_genes.at(i));
			}
		}
		else
		{
			if( orgm_1.Lconnection_genes.at(i).exist && orgm_2.Lconnection_genes.at(i).exist )
				(rand()%10 >= 5) ? orgm_resutl.add_connection(orgm_1.Lconnection_genes.at(i)) : orgm_resutl.add_connection(orgm_2.Lconnection_genes.at(i));
			else if ( orgm_1.Lconnection_genes.at(i).exist || orgm_2.Lconnection_genes.at(i).exist)
				(orgm_1.Lconnection_genes.at(i).exist ) ? orgm_resutl.add_connection(orgm_1.Lconnection_genes.at(i)) : orgm_resutl.add_connection(orgm_2.Lconnection_genes.at(i));
		}
	}
	//=============================================================

	return orgm_resutl;
}

// Spread the organisms along the niches 
void Population::spatiation()
{
	// Replace the previous niches by the current ones
	vector<Niche>().swap(prev_niches);
	prev_niches = current_niches;
	vector<Niche>().swap(current_niches);

	// Auxiliary variables
	vector <Niche> real_niches;
	vector <Niche> current_niches_temp;

	// Let's work with existing niches only
	// (so the probability of picking one always 
	//	will be valid)
	for (int i = 0; i < (int)prev_niches.size(); ++i)
		if (prev_niches[i].exist)
			real_niches.push_back(prev_niches[i]);

	// Prepare an empty niche
	Niche aux_niche;
	aux_niche.exist = false;

	// Fill a temp niche vector with the size of the real ones
	// with only empty niches
	for(int i=0; i < (int)real_niches.size(); i++)
		current_niches_temp.push_back(aux_niche);


	bool have_niche;
	int amount_of_new_niches = 0;

	// For every organism do ...
	for(int j = 0; j < (int)organisms.size(); j++)
	{
		have_niche = false;

		// Measure the distance with the champion of every real niche
		// Pick up the first match
		for (int i = 0; i < (int)real_niches.size(); ++i)
			if(compatibility(organisms[j], prev_organisms[real_niches[i].niche_champion_position]) < DISTANCE_THRESHOLD )
			{
				have_niche = true;
				current_niches_temp[i].exist = true;
				current_niches_temp[i].organism_position.push_back(j);

				break;
			}

		// Measure the distance with the champion of every new niche
		// Pick up the first match
		if (!have_niche)
		{
			for (int i = 0; i < amount_of_new_niches; ++i)
				if( compatibility( organisms[j], organisms[current_niches_temp[i + (int)real_niches.size()].niche_champion_position] ) < DISTANCE_THRESHOLD )
				{
					have_niche = true;
					current_niches_temp[i + (int)real_niches.size()].exist = true;
					current_niches_temp[i + (int)real_niches.size()].organism_position.push_back(j);
		
					break;
				}

			// If there were no matching then create a new niches
			if (!have_niche)
			{
				Niche aux2_niche;
				aux2_niche.exist = true;
				aux2_niche.niche_champion_position = j;
				aux2_niche.organism_position.push_back(j);
				current_niches_temp.push_back(aux2_niche);
				amount_of_new_niches++;
			}
		}
	}

	for (int i = 0; i < (int)current_niches_temp.size(); ++i)
		if( current_niches_temp[i].exist ) current_niches.push_back(current_niches_temp[i]);
}

void Population::SetFitness(double fitness, int organism_id)
{
	organisms.at(organism_id).fitness = fitness;
}

void Population::epoch()
{
	int mutation_amount; 
	int random_organism;
	int random_father;
	int random_mother; // for mating
	int random_niche_father;
	int random_niche_mother;

	Genetic_Encoding organism_temp;
	Genetic_Encoding organism_father;
	Genetic_Encoding organism_mother; // for mating

	double total_shared_fitness_population = 0.0;
	double temp_max_current_fitness = 0.0;

	// Calculates the fitness of each organism
	//for (int i = 0; i < (int)organisms.size(); ++i)
	//	organisms[i].fitness = fitness(organisms[i]);

	// Update all of the local champions and the general champion
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		current_niches[i].total_fitness = 0;

		for (int j = 0; j < (int)current_niches[i].organism_position.size(); ++j)
		{
			current_niches[i].total_fitness += organisms[current_niches[i].organism_position[j]].fitness;

			// ARREGLAR LO DEL J QUE SE VE FEITO
			//=====================================
			// Make the first the champion or
			// If this organism is better than the last one, use its place
			if(j == 0 || temp_max_current_fitness < organisms[current_niches[i].organism_position[j]].fitness)
			{
				temp_max_current_fitness = organisms[current_niches[i].organism_position[j]].fitness;
				current_niches[i].niche_champion_position = current_niches[i].organism_position[j];

				if(temp_max_current_fitness > fitness_champion)
				{
					fitness_champion = temp_max_current_fitness;
					champion = organisms[current_niches[i].organism_position[j]];
				}
			}
			//=====================================
		}
	}

	// Rationalize the fitness among the niches
	for (int i = 0; i < (int)current_niches.size(); ++i)
		total_shared_fitness_population += current_niches[i].total_fitness / (int)current_niches[i].organism_position.size();

	vector < Genetic_Encoding >().swap(prev_organisms);
	prev_organisms = organisms;
	vector < Genetic_Encoding >().swap(organisms);

	int sum = 0;
	
	// Calculate the amount of offspring of each niche
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		current_niches[i].amount_of_offspring = round(POPULATION_MAX*((current_niches[i].total_fitness/current_niches[i].organism_position.size())/total_shared_fitness_population));
		sum += current_niches[i].amount_of_offspring;
	}

	// If the proportional fitness of the current niche in the current organism is greater than the previous one
	// then replace the previous one with the champion's position
	for (int i = 0; i < (int)current_niches.size(); ++i)
		for (int j = 0; j < (int)current_niches[i].organism_position.size(); ++j)
			if(prev_organisms[current_niches[i].organism_position[j]].fitness < (current_niches[i].total_fitness / current_niches[i].organism_position.size()) )
				prev_organisms[current_niches[i].organism_position[j]] = prev_organisms[current_niches[i].niche_champion_position];

	//  
	for (int i = 0; i < (int)current_niches.size(); ++i)
		for (int j = 0; j < current_niches[i].amount_of_offspring; ++j)
		{
			// The champion always pass to the next generation
			if(j == 0) 
				organisms.push_back(prev_organisms[current_niches[i].niche_champion_position]);
			
			// Mutate some weights
			if(rand()%100 < PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER)
			{
				random_organism = rand() % current_niches[i].organism_position.size();
 				organism_temp = prev_organisms[random_organism];
				mutation_amount = rand() % (int)round(prev_organisms[random_organism].Lconnection_genes.size() * PERCENT_MUTATION_CONNECTION) + 1; 
				for(int k = 0; k < mutation_amount; k++)
					organism_temp = mutation_change_weight(organism_temp);
			}

			// NOT PRETTRY SURE WHAT IS THE DIFFERENCE BETWEEN THEESE TWO 
			//==========================================================			
			else
			{
				random_niche_father = i; // Is not really random

				if((rand()%1000)/1000.0 < PROBABILITY_INTERSPACIES_MATING)
				{	
					while(true)
					{
						random_niche_mother = rand()%current_niches.size();

						if(random_niche_mother != random_niche_father) break;
						if(current_niches.size() == 1 )
						{
							cerr << "Warning:: In function Epoch:: Exist only one niche\n";
							break;
						}
					}

					random_father = current_niches[random_niche_father].organism_position[rand()%current_niches[random_niche_father].organism_position.size()];
					organism_father = prev_organisms[random_father];

					random_mother = current_niches[random_niche_mother].organism_position[rand()%current_niches[random_niche_mother].organism_position.size()];
					organism_mother = prev_organisms[random_mother];

					organism_temp = crossover(organism_father, organism_mother);
				}
				else
				{
					random_father = current_niches[random_niche_father].organism_position[rand()%current_niches[random_niche_father].organism_position.size()];
					organism_father = prev_organisms[random_father];

					while(true)
					{
						random_mother = rand()%current_niches[random_niche_father].organism_position.size();

						if(random_mother != random_father) break;
						if(current_niches[random_niche_father].organism_position.size() == 1) break;
					}

					organism_mother = prev_organisms[random_mother];
					random_mother = current_niches[random_mother].organism_position[random_mother];

					organism_temp = crossover(organism_father, organism_mother);
				}
			}
			//==========================================================

			// CHANGE BY ()?: STATMENT 
			//========================
			// enter if is a small organism
			if( (int)organism_temp.Lnode_genes.size() <  LARGE_POPULATION_DISCRIMINATOR )
			{ 
				if((rand()%1000)/1000.0 < SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE)
					organism_temp = mutation_node(organism_temp);
				
				if((rand()%1000)/1000.0 < SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION) 
					organism_temp = mutation_connection(organism_temp);
			}

			// enter if is a large niche
			else
			{
				if((rand()%1000)/1000.0 < LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE)
					organism_temp = mutation_node(organism_temp);
				
				if((rand()%1000)/1000.0 < LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION) 
					organism_temp = mutation_connection(organism_temp);
			}
			//========================

			organisms.push_back(organism_temp);
		}

	spatiation();
}

#endif