#ifndef sma_calculate_hpp
#define sma_calculate_hpp

#include "common_includes.hpp"

using namespace std;

class smaCalc {

private:

	map<long int, double>* fp_sma_map;
	map<long int, double>* sp_sma_map;

public:

	smaCalc();
	~smaCalc();
	
	map<long int, double>* calculate_sma(vector<long int>* param_date, vector<double>* param_adjclose, int period, int call_flag);

};

#endif