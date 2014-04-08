#ifndef HYPERNEAT_CPP
#define HYPERNEAT_CPP

#include "HyperNeat.hpp"
#include "UserFunctions.hpp"
using namespace ANN_USM;

HyperNeat::HyperNeat(vector < double * > inputs, vector < double * > outputs, string hyperneat_info)
{
	substrate = new Substrate(inputs,outputs);

	cppn_neat = new Population();
	
	HJsonDeserialize(hyperneat_info);

	clog << "\t-> Deserialize ok!" << endl;
}
HyperNeat::~HyperNeat()
{
	free(substrate);
	free(cppn_neat);
	vector<CPPNInputs>().swap(AditionalCPPNInputs);
	vector<double>().swap(CppnInputs);
}

void HyperNeat::HJsonDeserialize(string hyperneat_info)
{
	char str[(int)hyperneat_info.size()];
	strcpy(str, hyperneat_info.c_str());
	const char delimeters[] = "{\"\t\n:,[ ]}";
	char *pch = strtok(str, delimeters);

	while(pch != NULL)
	{
		if (!strcmp(pch,(char *)"Genetic_Encoding")){
			pch = strtok(NULL, delimeters);
			cppn_neat->CJsonDeserialize(pch);
			pch = strtok(NULL, delimeters);
			break;
		}else{ 
			if (!strcmp(pch,(char *)"Substrate")){	
				pch = strtok(NULL, delimeters);
				pch = substrate->SJsonDeserialize(pch);
				//pch = strtok(NULL, delimeters);
			}else{
				if(!strcmp(pch,(char *)"connection_threshold")){
					pch = strtok(NULL, delimeters);
					connection_threshold = atof(pch);
					pch = strtok(NULL, delimeters);
				}else{
					if(!strcmp(pch,(char *)"AditionalCPPNInputs")){
						for(int i = 0; i < n_AditionalCPPNInputs; i++){
							pch = strtok(NULL, delimeters);		
							if (!strcmp(pch,(char *)"BIAS")){	
								char * aux = pch;
								pch = strtok(NULL, delimeters);						
								AditionalCPPNInputs.push_back(CPPNInputs(aux, atof(pch)));
							}else																
								AditionalCPPNInputs.push_back(CPPNInputs(pch, 0.0));
						}						
						pch = strtok(NULL, delimeters);					
					}else{
						if(!strcmp(pch,(char *)"n_AditionalCPPNInputs")){
							pch = strtok(NULL, delimeters);
							n_AditionalCPPNInputs = atoi(pch);
							pch = strtok(NULL, delimeters);
						}
					}					
				}						
			}
		}
	}
}
bool HyperNeat::CreateSubstrateConnections(int organism_id)
{
	if(substrate->GetLayoutNumber() == 0)
	{
		cout << "Does not exist any substrate initialized" << endl;
		return false;
	}

	if(cppn_neat->champion.output_nodes.size() > 1 )
	{
		vector < vector < double > > coord;
		vector < vector < double > > cppn_output;
		bool flag;

		if(substrate->GetLayoutNumber() > 1)
		{
			if((int)cppn_neat->champion.output_nodes.size() != substrate->GetLayoutNumber()-1)
			{
				cout << "The layout number does not correspond to the cppn output number" << endl;
				return false;
			}

			n_connections = vector < int > (substrate->GetLayoutNumber()-1,0);

			for(int i = 0; i < substrate->GetLayoutNumber()-1; i++)
			{				
				substrate->ClearSpatialNodeInputs(i+1,0);

				for(int j = 0; j < substrate->GetLayerNodesNumber(i,0); j++)
					for(int k = 0; k < substrate->GetLayerNodesNumber(i+1,0); k++)
					{
						flag = true;
						vector < double > cppn_inputs;
						int c = 0;
						vector < double > c1 = (substrate->GetSpatialNode(i,0,j))->GetCoordenates();
						vector < double > c2 = (substrate->GetSpatialNode(i+1,0,k))->GetCoordenates();
						cppn_inputs.insert(cppn_inputs.end(), c1.begin(), c1.end());
						cppn_inputs.insert(cppn_inputs.end(), c2.begin(), c2.end());
						vector < double > input_aux (cppn_inputs);

						for(int c = 0; c < n_AditionalCPPNInputs; c++)
							cppn_inputs.push_back(AditionalCPPNInputs[c].Eval(input_aux));

						for(c = 0; c < (int)coord.size(); c++)
							if(coord.at(c) == cppn_inputs)
							{
								if(abs(cppn_output.at(c).at(i)) > connection_threshold)
								{
									(substrate->GetSpatialNode(i+1,0,k))->AddInputToNode(substrate->GetSpatialNode(i,0,j), cppn_output.at(c).at(i));
									n_connections.at(i)++;
								}
								flag = false;
								continue;
							}

						if(flag)
						{
							coord.push_back(cppn_inputs);
							cppn_output.push_back(cppn_neat->CalculeWeight(cppn_inputs, organism_id));

							if(abs(cppn_output.back().at(i)) > connection_threshold)
							{
								(substrate->GetSpatialNode(i+1,0,k))->AddInputToNode(substrate->GetSpatialNode(i,0,j), cppn_output.back().at(i));
								n_connections.at(i)++;
							}
						}
					}
			}		
			
		}
		else
		{			
				cout << "The layout number must be greater than zero to use multiple cppn-neat outputs" << endl;
				return false;
		}

	}
	else
	{
		if(substrate->GetLayoutNumber() > 1)
		{
			n_connections = vector < int > (substrate->GetLayoutNumber()-1,0);

			for(int i = 0; i < substrate->GetLayoutNumber()-1; i++)
			{
				substrate->ClearSpatialNodeInputs(i+1,0);

				for(int j = 0; j < substrate->GetLayerNodesNumber(i,0); j++)
				{
					for(int k = 0; k < substrate->GetLayerNodesNumber(i+1,0); k++)
					{
						vector < double > cppn_inputs;
						vector < double > c1 = (substrate->GetSpatialNode(i,0,j))->GetCoordenates();
						vector < double > c2 = (substrate->GetSpatialNode(i+1,0,k))->GetCoordenates();
						cppn_inputs.insert(cppn_inputs.end(), c1.begin(), c1.end());
						cppn_inputs.insert(cppn_inputs.end(), c2.begin(), c2.end());
						vector < double > input_aux (cppn_inputs);

						for(int c = 0; c < n_AditionalCPPNInputs; c++)
							cppn_inputs.push_back(AditionalCPPNInputs[c].Eval(input_aux));
						cout << "Antes CW" << endl;
						double weight = (cppn_neat->CalculeWeight(cppn_inputs, organism_id)).at(0);
						cout << "Despues CW" << endl;
						if(abs(weight) > connection_threshold)
						{
							(substrate->GetSpatialNode(i+1,0,k))->AddInputToNode(substrate->GetSpatialNode(i,0,j), weight);
							n_connections.at(i)++;
						}
					}
				}
			}
		}
		else
		{
			n_connections = vector < int > (substrate->GetLayersNumber(0)-1,0);

			for(int i = 0; i < substrate->GetLayersNumber(0) - 1; i++)
			{
				substrate->ClearSpatialNodeInputs(0,i+1);

				for(int j = 0; j < substrate->GetLayerNodesNumber(0,i); j++)
				{
					for(int k = 0; k < substrate->GetLayerNodesNumber(0,i+1); k++)
					{
						vector < double > cppn_inputs;
						vector < double > c1 = (substrate->GetSpatialNode(0,i,j))->GetCoordenates();
						vector < double > c2 = (substrate->GetSpatialNode(0,i+1,k))->GetCoordenates();
						cppn_inputs.insert(cppn_inputs.end(), c1.begin(), c1.end());
						cppn_inputs.insert(cppn_inputs.end(), c2.begin(), c2.end());
						vector < double > input_aux (cppn_inputs);

						for(int c = 0; c < n_AditionalCPPNInputs; c++)
							cppn_inputs.push_back(AditionalCPPNInputs[c].Eval(input_aux));

						double weight = (cppn_neat->CalculeWeight(cppn_inputs, organism_id)).at(0);
						

						if(abs(weight) > connection_threshold)
						{
							(substrate->GetSpatialNode(0,i+1,k))->AddInputToNode(substrate->GetSpatialNode(0,i,j), weight);
							n_connections.at(i)++;
						}
					}
				}
			}
		}
	}
	for(int i = 0; i < (int)n_connections.size(); i++)
	{
		if(n_connections.at(i) < 1) return false;
	}
	return true;
	
}

bool HyperNeat::EvaluateSubstrateConnections()
{
	if(substrate->GetLayoutNumber() == 0){
		cout << "Does not exist any substrate initialized" << endl;
		return false;
	}
	for(int i = 0; i < (int)n_connections.size(); i++)
		if(n_connections.at(i) == 0){		
			cout << "Does not exist any connection initialized between the sheet " << i << " and " << i+1 << endl;
			return false;
		}

	if(substrate->GetLayoutNumber() > 1)
		for(int i = 0; i < substrate->GetLayoutNumber(); i++)
			substrate->EvaluateSpatialNode(i,0);
	else
		for(int i = 0; i < substrate->GetLayersNumber(0);i++)
			substrate->EvaluateSpatialNode(0,i);

	return true;
}

void HyperNeat::HyperNeatFitness(double fitness, int organism_id)
{
	cppn_neat->SetFitness(fitness, organism_id);
}

void HyperNeat::HyperNeatEvolve()
{
	cppn_neat->epoch();
}

void HyperNeat::GetHyperNeatOutputFunctions(string plataform)
{
	CreateSubstrateConnections(-1);
	EvaluateSubstrateConnections();

	vector < string > OUTPUTS;

	OUTPUTS = substrate->GetSubstrateOutputFunctions(plataform);

	if(!strcmp(plataform.c_str(),(char *)"octave"))
	{	
		stringstream file_name;
		file_name << "functions_files/" << HYPERNEAT_TEST << ".m";
		ofstream myfile (file_name.str().c_str());

		GetNodeFunction(plataform);

		if (myfile.is_open()){

			myfile << "function [ ";
			for(int i = 0; i < (int)substrate->outputs.size(); i++){
				myfile << "OUTPUT_" << i ;
				if(i < (int)substrate->outputs.size()-1) myfile << ", ";
			}
			myfile << " ] = " << HYPERNEAT_TEST << "( ";
			for(int i = 0; i < (int)substrate->inputs.size(); i++){
				myfile << "INPUT_" << i ;
				if(i < (int)substrate->inputs.size()-1) myfile << ", ";
			}
			myfile << " )" << endl;
			for(int i = 0; i < (int)substrate->outputs.size(); i++)
				myfile << OUTPUTS[i] << ";" << endl;
		    myfile.close();
	  	}else 
	  		cerr << "Unable to open file: " << file_name.str() << endl;
	}
	else if(!strcmp(plataform.c_str(),(char *)"c++"))
	{
		ofstream myfile ("functions_files/HYPERNEAT_FUNCTIONS.hpp");

		if (myfile.is_open()){

			myfile << "#ifndef HYPERNEAT_FUNCTIONS_H" << endl;
			myfile << "#define HYPERNEAT_FUNCTIONS_H" << endl << endl;
			myfile << "#include < math.h >" << endl;
			myfile << SIGMOID_STRING << endl;

			for(int i = 0; i < (int)substrate->outputs.size(); i++)
			{
				myfile << "#define " << HYPERNEAT_TEST << "_" << i << "(";

				for(int j = 0; j < (int)substrate->inputs.size(); j++)
				{
				 	myfile << "INPUT_" << j;
				 	if(j < (int)substrate->inputs.size()-1) myfile << ", ";
				}

				myfile << ") " << OUTPUTS[i] << endl;

			}

			myfile << endl << "#endif" << endl;
		    myfile.close();
			
	  	}else 
	  		cerr << "Unable to open file: HYPERNEAT_FUNCTIONS" << endl;
	}
	
}

#endif