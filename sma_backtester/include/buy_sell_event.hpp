#ifndef buy_sell_event_hpp
#define buy_sell_event_hpp

#include "common_includes.hpp"

using namespace std;

class bsEvent {

private:
	int enum_Obj;
	map<long int, int>* m_bsEvent;
	map<long int, double>* m_transaction;
	map<long int, float>* m_returns;
	map<string, float>* m_performanceMetrics;

public:

	bsEvent();
	map<long int, int>* getBSEvent(const map<long int, double>* fpsma_map, const map<long int, double>* spsma_map);

	map<long int, double>* getTransaction(const vector<long int>* param_date, const map<long int, double>* param_adjclose);

	map<long int, float>* getReturns();

	map<string, float>* getPerformanceMetrics();

	~bsEvent();
};


#endif