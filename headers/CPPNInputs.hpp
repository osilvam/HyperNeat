#ifndef CPPNINPUTS_H
#define CPPNINPUTS_H

#include <cmath>
#include <vector>
#include <cstring>

using namespace std;

namespace ANN_USM{
	class CPPNInputs
	{
		//POR AHORA SOLO PARA 2D!!!!!!!
		double bias;
		char * type;
		double (CPPNInputs::*function)(vector < double > point);
		double Bias(vector < double > point);
		double Radius1(vector < double > point);
		double Radius2(vector < double > point);	
		double EuclidianDistance(vector < double > point);		
		//double user_custom(vector<double> points);
	public:
		CPPNInputs(char * type, double bias);
		~CPPNInputs();		
		double Eval(vector < double > point);
		char * GetType();
	};
}

#endif