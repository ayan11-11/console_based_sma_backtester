#include "sma_calculate.hpp"


smaCalc::smaCalc() {
	fp_sma_map = new map<long int, double>();
	sp_sma_map = new map<long int, double>();
}

smaCalc::~smaCalc() {
	delete fp_sma_map;
	delete sp_sma_map;
}

map<long int, double>* smaCalc::calculate_sma(vector<long int>* param_date, vector<double>* param_adjclose, int period, int call_flag) {

	if(call_flag == 1) {
		fp_sma_map->clear();
	}else {
		sp_sma_map->clear();
	}

	vector<double>::const_iterator itv;
	
	/*
	 * The std::vector::data() is an STL in C++ 
	 * which returns a direct pointer to the memory 
	 * array used internally by the vector to store 
	 * its owned elements.
	 */

	long int* d_pos = param_date->data();
	double* adjc_pos = param_adjclose->data();
	
	int temp_period = 0;
	int startpoint = 0;
	double sum = 0;
	int num_input = period;
	double sma = 0;

	for(itv = param_adjclose->begin(); itv != param_adjclose->end(); ++itv){

		for(startpoint = temp_period;startpoint < period; startpoint++) {
			sum += *(adjc_pos + startpoint);
		}

		sma = sum/num_input;
		temp_period++;
		
		sum = 0;

		// cout << "size of vector: "<< param_adjclose->capacity()<<" " << period <<" "<<*(d_pos + temp_var)<< " " << sma << endl;
		if(call_flag == 1) {
			fp_sma_map->insert(make_pair(*(d_pos + period), sma));
		}else {
			sp_sma_map->insert(make_pair(*(d_pos + period), sma));
		}
		
		period++;

		if(period == param_adjclose->capacity()) break;

	}

	if(call_flag == 1) {
		return fp_sma_map;
	}else {
		return sp_sma_map;
	}
}