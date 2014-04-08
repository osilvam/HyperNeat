// Copyright 2006-2014 Dr. Marc Andreas Freese. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// -------------------------------------------------------------------
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// 
// You are free to use/modify/distribute this file for whatever purpose!
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.1.0 on January 20th 2014

// Make sure to have the server side running in V-REP!
// Start the server from a child script with following command:
// simExtRemoteApiStart(portNumber) -- starts a remote API server service on the specified port

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <time.h>
#include <sys/time.h>
#include <fstream>
#include "HyperNeat.hpp"
#include "UserFunctions.hpp"

using namespace std;

#define PORTNB 19998
#define N_LEGS 4
#define GRA_LIB 2
#define GRA_LIB_EXT 1
#define ADITIONAL_HYPERNEAT_INPUTS 2
#define TIME_SIMULATION 12.0
#define RAD (double)M_PI/180
#define MAX_ANGLE_LIMIT {60.0*RAD,60.0*RAD,60.0*RAD,60.0*RAD,39.0*RAD,39.0*RAD,39.0*RAD,39.0*RAD,23.0*RAD}
#define MIN_ANGLE_LIMIT {-85.0*RAD,-85.0*RAD,-85.0*RAD,-85.0*RAD,-113.0*RAD,-113.0*RAD,-113.0*RAD,-113.0*RAD,-23.0*RAD}

#define FRECUENCY_CMD 160.0
#define STEP_CALC 4.0
#define FRECUENCY_THRESHOLD 1.67
#define WAVE_FRECUENCY 0.64
#define SIN(X,PHI) (double)sin(2.0*M_PI*WAVE_FRECUENCY*X + PHI*M_PI/2.0)
#define F_DISTANCE_PENALIZATION(X) (double)exp(FRECUENCY_THRESHOLD-X)
#define FITNESS_FUNCTION(X) (double)pow(2,pow(X,2))
#define OUTPUT_CONVERSION(X1,X2,Y1,Y2,X) (double)((Y2-Y1)*(X-X1)/(X2-X1)+Y1) 

extern "C" {
    #include "extApi.h"
/*	#include "extApiCustom.h" if you wanna use custom remote API functions! */
}

double mean(vector < double > numbers)
{	
	if((int)numbers.size() == 0) return 0.0;

	double average = 0;

	for(int i = 0; i < (int)numbers.size(); i++)
		average += numbers.at(i);
	
	return (double)average/numbers.size();
}

double stdDesviation(vector < double > numbers)
{
	if((int)numbers.size() == 0) return 0.0;

	double average = mean(numbers);
	double sum = 0;

	for(int i = 0; i < (int)numbers.size(); i++)
		sum += pow(numbers.at(i)-average,2);
	
	return sqrt(sum/numbers.size());

}

double output_conversion(double X1, double X2, double Y1, double Y2, double X)
{
	if(X2-X1 != 0) return (Y2-Y1)*(X-X1)/(X2-X1)+Y1;
	else return 0.0;

}

double getTimeSimulation(timeval final, timeval init)
{
	return (double)((final.tv_sec-init.tv_sec)*1000.0+(final.tv_usec-init.tv_usec)/1000.0)/1000.0;
}

int main(int argc,char* argv[])
{
	srand (time(0));
	int percentage = 0;
	timeval init;
	timeval final;

	ofstream vrep_error;
	ofstream vrep_position;
	ofstream vrep_outputs;
	ofstream train_results;
	ofstream fitness_results;
	ofstream frecuency_results;

	vrep_error.open("error_files/vrep_error.txt");
	vrep_outputs.open("simulation_files/vrep_outputs.txt");
	train_results.open("simulation_files/train_results.txt");
	fitness_results.open("simulation_files/quadratot_fitness.txt");
	frecuency_results.open("simulation_files/quadratot_frecuency.txt");

	if(argc < 2) return -1;

	// ============================= READING JSON FILE ============================= //
	ifstream file;
	file.open(argv[1]);
	string jsonstring((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	// ============================================================================= //	

	int clientID = simxStart((simxChar*)"127.0.0.1",PORTNB,true,true,2000,5);	
	//vrep_error << "simxLoadScene error: " << simxLoadScene(clientID, "/home/oscar/NEW_QUADRATOT_VREP.ttt", '0', simx_opmode_oneshot_wait) << endl;

	// ============= VREP INITIALIZATIONS ============= //			

	vector < int > joints;
	vector < int > tips;
	int center_dummy;

	vrep_error << "JOINT HANDLES INITIALIZATION" << endl;
	vrep_outputs << "JOINT HANDLES INITIALIZATION" << endl;

	for(int i = 0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; i++)
	{
		stringstream joint;
		joint << "joint" << i;
		int handle;

		vrep_error << "simxGetObjectHandle error: " << simxGetObjectHandle(clientID, joint.str().c_str(), &handle, simx_opmode_oneshot_wait) << endl;
		vrep_outputs << joint.str() << " handle: " << handle << endl;

		joints.push_back(handle);
	}

	vrep_error << endl;
	vrep_outputs << endl;

	vrep_error << "TIP HANDLES INITIALIZATION" << endl;
	vrep_outputs << "TIP HANDLES INITIALIZATION" << endl;

	for(int i = 0; i < N_LEGS; i++)
	{			
		stringstream tip;
		tip << "tip" << i;
		int handle;

		vrep_error << "simxGetObjectHandle error: " << simxGetObjectHandle(clientID, tip.str().c_str(), &handle, simx_opmode_oneshot_wait) << endl;
		vrep_outputs << tip.str() << " handle: " << handle << endl;

		tips.push_back(handle);
	}

	vrep_error << "simxGetObjectHandle error: " << simxGetObjectHandle(clientID, "center", &center_dummy, simx_opmode_oneshot_wait) << endl;
	vrep_outputs << "center_dummy handle: " << center_dummy << endl;

	vrep_error << endl;
	vrep_outputs << endl;

	// ================================================ //

	// ========== HYPERNEAT INITIALIZATIONS =========== //

	vector < double * > next;
	vector < double * > pass;
	vector < double > init_pos;

	vector < double > pass_pass_pos;
	vector < double > pass_pos;
	vector < double > next_pos;

	double max_angle_limit[] = MAX_ANGLE_LIMIT;
	double min_angle_limit[] = MIN_ANGLE_LIMIT;

	double fitness = 0;
	double distance = 0;
	int jdcn = 0;//joint direction change number

	vrep_error << "HYPERNEAT INPUTS & OUTPUTS INITIALIZATION" << endl;
	vrep_outputs << "HYPERNEAT INPUTS & OUTPUTS INITIALIZATION" << endl;

	for(int i = 0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; i++)
	{
		float joint_pos;

		vrep_error << "simxGetJointPosition error: " << simxGetJointPosition(clientID, joints.at(i), &joint_pos, simx_opmode_oneshot_wait) << endl;
		vrep_outputs << "joint" << i << " handle: " << joints.at(i) << " angle: " << joint_pos << endl;

		//joint_pos = (float)OUTPUT_CONVERSION(min_angle_limit[i],max_angle_limit[i],-1.0,1.0,(double)joint_pos);
		//vrep_outputs << " conversion: " << joint_pos << endl;
		double * joint_next_pos = new double((double)OUTPUT_CONVERSION(min_angle_limit[i],max_angle_limit[i],-1.0 ,1.0, joint_pos));
		double * joint_pass_pos = new double((double)OUTPUT_CONVERSION(min_angle_limit[i],max_angle_limit[i],-1.0 ,1.0, joint_pos));

		next.push_back(joint_next_pos);
		pass.push_back(joint_pass_pos);
		init_pos.push_back((double)joint_pos);

		pass_pass_pos.push_back((double)joint_pos);
		pass_pos.push_back((double)joint_pos);
		next_pos.push_back((double)joint_pos);
	}

	for(int i = 0; i < ADITIONAL_HYPERNEAT_INPUTS; i++)
	{
		double * aux_pass = new double(SIN(0,i));
		pass.push_back(aux_pass);
	}

	vrep_error << endl;
	vrep_outputs << endl;

	HyperNeat * hyperneat = new HyperNeat(pass, next, jsonstring);

	// ================================================ //

	if (clientID!=-1)
	{
		int g, p;

		while (simxGetConnectionId(clientID)!=-1)
		{
			clog << "\t-> Training    ";

			for (g = 0; g < GENERATIONS; g++)
			{
				vector < double > generation_fitness;
				vector < double > generation_frecuency;
				double fitness_stdD = 0;
				double fitness_mean = 0;
				double frecuency_stdD = 0;
				double frecuency_mean = 0;

				// ========== PRINTING COMPLETION PERCENTAGE ========== //
				percentage = ((double)g / (GENERATIONS - 1)) * 100;

				clog << "\b";
				for (int i = percentage; i > 0 ; i/=10)
					clog << "\b";

				clog << percentage << "\%";
				// ==================================================== //

				for(p = 0; p < POPULATION_MAX; p++)
				{
					cout << "G_" << g << " P_" << p << endl;
					if(!hyperneat->CreateSubstrateConnections(p)) continue;
					cout << "EXITO" << endl;
					stringstream file_name_2;
					file_name_2 << "simulation_files/distance_result/quadratot_position_G" << g << "_P" << p << ".txt";
					vrep_position.open(file_name_2.str().c_str());
					float pass_position[3];
					float position[3];
					jdcn = 0;
					vector < double > sum_next ((int)joints.size(),0.0);
					vector < double > slope_position;
					int step = 0;

					// ============= SETING INITIAL HYPERNEAT INPUTS ============= //

					for(int i = 0; i < (int)joints.size(); i++)
					{
						*next.at(i) = init_pos.at(i);
						*pass.at(i) = init_pos.at(i);

						pass_pass_pos.at(i) = init_pos.at(i);
						pass_pos.at(i) = init_pos.at(i);
						next_pos.at(i) = init_pos.at(i);
					}

					// ============================================================ //

					vrep_error << "simxStartSimulation error: " << simxStartSimulation(clientID, simx_opmode_oneshot_wait) << endl;

					gettimeofday(&init,NULL);
					gettimeofday(&final,NULL);

					while(getTimeSimulation(final,init) < TIME_SIMULATION)
					{
						hyperneat->EvaluateSubstrateConnections();

						if(step >= STEP_CALC)
						{
							for(int i = 0; i < (int)joints.size(); i++)
							{
								next_pos.at(i) = OUTPUT_CONVERSION(-1.0,1.0,min_angle_limit[i],max_angle_limit[i],(double)sum_next.at(i)/STEP_CALC);

								if( !(( pass_pass_pos.at(i) <= pass_pos.at(i) && pass_pos.at(i) <= next_pos.at(i) ) || ( pass_pass_pos.at(i) >= pass_pos.at(i) && pass_pos.at(i) >= next_pos.at(i) )) )
									jdcn++;

								pass_pass_pos.at(i) = pass_pos.at(i);
								pass_pos.at(i) = next_pos.at(i);	
								*pass.at(i) = (double)sum_next.at(i)/STEP_CALC;
								sum_next.at(i) = 0;			
							}

							for(int i = 0; i < ADITIONAL_HYPERNEAT_INPUTS; i++)
							{
								*pass.at((int)joints.size()+i) = SIN(getTimeSimulation(final,init),i);
							}

							// ============== SET JOINT TARGET POSITION FOR EACH INTERATION ============== //

							vrep_error << "SETING JOINT POSITIONS" << endl;

							simxPauseCommunication(clientID,1);

							for(int i = 0; i < (int)joints.size(); i++)
							{
								vrep_error << "simxSetJointTargetPosition error: " << simxSetJointTargetPosition(clientID, joints.at(i), (float)next_pos.at(i), simx_opmode_oneshot) << endl;
							}

							simxPauseCommunication(clientID,0);

							vrep_error << endl;

							vrep_error << "simxGetObjectPosition error: " << simxGetObjectPosition(clientID, center_dummy,-1, position, simx_opmode_oneshot) << endl;
							vrep_position << position[0] << "\t" << position[1] << "\t" << getTimeSimulation(final,init) << endl;

							// =========================================================================== //
							pass_position[0] = position[0];
							pass_position[1] = position[1];

							if(!(pass_position[0] == position[0] && pass_position[1] == position[1]))
								slope_position.push_back((position[0]-pass_position[0])/(position[1]-pass_position[1]));

							step = 0;
						}
						else
						{
							for(int i = 0; i < (int)joints.size(); i++)
							{
								sum_next.at(i) = sum_next.at(i) + *next.at(i);
								*pass.at(i) = *next.at(i);
							}		
							step++;				
						}

						usleep((int)((1.0/FRECUENCY_CMD)*1000000.0/STEP_CALC));
						gettimeofday(&final,NULL);
					}					

					vrep_error << "simxStopSimulation error: " << simxStopSimulation(clientID, simx_opmode_oneshot_wait) << endl;

					vrep_position.close();

					// ============= FITNESS CALCULE ============= //

					double d;
					double frecuency = (double)(jdcn/N_LEGS)/TIME_SIMULATION;
					cout << jdcn << endl;

					// ATENCION!! CALCULO DE DISTANCIA PROVISORIO
					distance = sqrt(pow(position[0],2) + pow(position[1],2));
					d = distance*F_DISTANCE_PENALIZATION(frecuency);
					fitness = FITNESS_FUNCTION(d);

					// =========================================== //

					train_results << endl << "===========================================   G" << g << " P" << p <<endl;
					train_results << "Joint distance change number frecuency: " << frecuency << endl;
					train_results << "Traveled distance : " << distance << endl;
					train_results << "Penalized distance : " << d << endl;
					train_results << "Fitness: " << fitness << endl;

					generation_fitness.push_back(fitness);
					generation_frecuency.push_back(frecuency);

					hyperneat->HyperNeatFitness(fitness, p);
				}

				fitness_stdD = stdDesviation(generation_fitness);
				fitness_mean = mean(generation_fitness);

				frecuency_stdD = stdDesviation(generation_frecuency);
				frecuency_mean = mean(generation_frecuency);

				fitness_results << fitness_mean << "\t" << fitness_stdD << "\t" << g << endl;
				frecuency_results << frecuency_mean << "\t" << frecuency_stdD << "\t" << FRECUENCY_THRESHOLD << "\t" << g << endl;

				hyperneat->HyperNeatEvolve();

			}			
			
			break;
		}

		if(g < GENERATIONS || p < POPULATION_MAX)
		{
			clog << endl << endl << "Training Failed!" << endl;
			return(0);
		} 
		else clog << " ok!" << endl;

		clog << endl << "BEST RESULT ------------------------------------" << endl << endl;
		clog << "\t-> " << hyperneat->cppn_neat->champion.fitness << endl << endl;

	}

	//OBTEIN FUNCTIONS
	hyperneat->GetHyperNeatOutputFunctions("c++");

	char run;
	clog << "\tWant you run the simulation with the champion organism (S/N): ";
	cin >> run;

	if(run == 's' || run == 'S')
	{
		if(!hyperneat->CreateSubstrateConnections(-1)) return(0);

		stringstream file_name_2;
		file_name_2 << "simulation_files/distance_result/quadratot_champion.txt";
		vrep_position.open(file_name_2.str().c_str());
		float position[3];

		// ============= SETING INITIAL HYPERNEAT INPUTS ============= //

		for(int i = 0; i < (int)joints.size(); i++)
		{
			*next.at(i) = init_pos.at(i);
			*pass.at(i) = init_pos.at(i);

			pass_pass_pos.at(i) = init_pos.at(i);
			pass_pos.at(i) = init_pos.at(i);
			next_pos.at(i) = init_pos.at(i);
		}

		// ============================================================ //

		vrep_error << "simxStartSimulation error: " << simxStartSimulation(clientID, simx_opmode_oneshot_wait) << endl;

		gettimeofday(&init,NULL);
		gettimeofday(&final,NULL);

		while(getTimeSimulation(final,init) < TIME_SIMULATION)
		{
			hyperneat->EvaluateSubstrateConnections();

			for(int i = 0; i < (int)joints.size(); i++)
			{
				next_pos.at(i) = OUTPUT_CONVERSION(-1.0,1.0,min_angle_limit[i],max_angle_limit[i],*next.at(i));

				pass_pass_pos.at(i) = pass_pos.at(i);
				pass_pos.at(i) = next_pos.at(i);	
				*pass.at(i) = next_pos.at(i);			
			}

			for(int i = 0; i < ADITIONAL_HYPERNEAT_INPUTS; i++)
			{
				*pass.at((int)joints.size()+i) = SIN(getTimeSimulation(final,init),i);
			}

			// ============== SET JOINT TARGET POSITION FOR EACH INTERATION ============== //

			vrep_error << "SETING JOINT POSITIONS" << endl;

			simxPauseCommunication(clientID,1);

			for(int i = 0; i < (int)joints.size(); i++)
			{
				vrep_error << "simxSetJointTargetPosition error: " << simxSetJointTargetPosition(clientID, joints.at(i), (float)next_pos.at(i), simx_opmode_oneshot) << endl;
			}

			simxPauseCommunication(clientID,0);

			vrep_error << endl;

			vrep_error << "simxGetObjectPosition error: " << simxGetObjectPosition(clientID, center_dummy,-1, position, simx_opmode_oneshot) << endl;
			vrep_position << position[0] << "\t" << position[1] << "\t" << getTimeSimulation(final,init) << endl;
			
			// =========================================================================== //
			
			usleep((int)((1.0/FRECUENCY_CMD)*1000000.0));						

			gettimeofday(&final,NULL);
		}		

		vrep_error << "simxStopSimulation error: " << simxStopSimulation(clientID, simx_opmode_oneshot_wait) << endl;

		vrep_position.close();
			
	}

	vrep_error.close();
	vrep_outputs.close();
	train_results.close();
	fitness_results.close();
	frecuency_results.close();

	simxFinish(clientID);

	return(0);
}



