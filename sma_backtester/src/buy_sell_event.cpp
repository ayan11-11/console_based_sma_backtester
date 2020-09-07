#include "buy_sell_event.hpp"


bsEvent::bsEvent() {
	m_bsEvent = new map<long int, int> ();
	m_transaction = new map<long int, double>();
	m_returns = new map<long int, float>();
	m_performanceMetrics = new map<string, float>();
}

bsEvent::~bsEvent() {
	delete m_bsEvent;
	delete m_transaction;
	delete m_returns;
	delete m_performanceMetrics;
}

map<long int, int>* bsEvent::getBSEvent(const map<long int, double>* fpsma_map, const map<long int, double>* spsma_map) {

	map<long int, double>::const_iterator sp_itm1 = spsma_map->begin();
	map<long int, double>::const_iterator sp_itm2;
	
	map<long int, double>::const_iterator fp_itm1 = (fpsma_map->find(sp_itm1->first));
	map<long int, double>::const_iterator fp_itm2;

	double fpsma_value1 = 0;
	double fpsma_value2 = 0;
	double spsma_value1 = 0;
	double spsma_value2 = 0;	

	for(sp_itm1,fp_itm1; sp_itm1 != spsma_map->end() && fp_itm1 != fpsma_map->end(); sp_itm1++,fp_itm1++) {

		fp_itm2 = fp_itm1;
		sp_itm2 = sp_itm1;

		fp_itm2--;
		

		fpsma_value1 = fp_itm1->second;
		fpsma_value2 = fp_itm2->second;
		spsma_value1 = sp_itm1->second;
		if(sp_itm2 == spsma_map->begin()) {
			spsma_value2 = 0;
		}else {
			sp_itm2--;
			spsma_value2 = sp_itm2->second;
		}

		if((fpsma_value1 > spsma_value1) && (fpsma_value2 < spsma_value2)) {
			m_bsEvent->insert(make_pair(sp_itm1->first,1)); // 1 for buy
		}
		else if((fpsma_value1 < spsma_value1) && (fpsma_value2 > spsma_value2)){
			m_bsEvent->insert(make_pair(sp_itm1->first,2)); // 2 for sell
		}
	} 
	return m_bsEvent;
}


map<long int, double>* bsEvent::getTransaction(const vector<long int>* param_date, const map<long int, double>* param_adjclose) {

	double transaction_value = 0;

	vector<long int>::const_iterator d_itv = param_date->begin();

	for(d_itv; d_itv != param_date->end(); d_itv++) {

		m_transaction->insert(make_pair(*d_itv, transaction_value));

		auto bse_itm = m_bsEvent->find(*d_itv);

		if(bse_itm != m_bsEvent->end()) {
			auto adc_itm = param_adjclose->find(bse_itm->first);
			adc_itm++;
			transaction_value = adc_itm->second;
		}

	}

	return m_transaction;
}


map<long int, float>* bsEvent::getReturns() {

	float return_value = 0;

	for(auto bse_itm = m_bsEvent->begin(); bse_itm != m_bsEvent->end(); bse_itm++) {

		if(bse_itm->second == 1) {
			auto tran_itm = m_transaction->find(bse_itm->first);
			auto denominator = tran_itm->second;
			tran_itm++;
			auto numerator = tran_itm->second;

			if(denominator != 0) {
				return_value = ((1 - (numerator/denominator)) * 100);
			}else {
				return_value = 0;
			}

			m_returns->insert(make_pair(tran_itm->first, return_value));
		}
		else if(bse_itm->second == 2) {
			auto tran_itm = m_transaction->find(bse_itm->first);
			auto denominator = tran_itm->second;
			tran_itm++;
			auto numerator = tran_itm->second;

			if(denominator != 0) {
				return_value = (((numerator/denominator) - 1) * 100);
			} else {
				return_value = 0;
			}

			m_returns->insert(make_pair(tran_itm->first, return_value));
		}
	}



	return m_returns;
}

map<string, float>* bsEvent::getPerformanceMetrics() {

	float positive_returns = 0;
	float negative_returns = 0;
	float positive_trades = 0;
	float negative_trades = 0;
	float hit_ratio = 0;
	float average_returns = 0;

	for(auto ret_itm = m_returns->begin(); ret_itm != m_returns->end(); ret_itm++) {
		if(ret_itm->second > 0) {
			positive_returns += ret_itm->second;
			positive_trades++;
		}
		else if(ret_itm->second < 0) {
			negative_returns += ret_itm->second;
			negative_trades++;
		}
	}

	hit_ratio = positive_trades/(positive_trades + negative_trades);

	average_returns = (positive_returns + negative_returns)/(positive_trades + negative_trades);

	m_performanceMetrics->insert(make_pair("Positive_Returns",positive_returns));
	m_performanceMetrics->insert(make_pair("Negative_Returns",negative_returns));
	m_performanceMetrics->insert(make_pair("Positive_Trades",positive_trades));
	m_performanceMetrics->insert(make_pair("Negative_Trades",negative_trades));
	m_performanceMetrics->insert(make_pair("Hit_Ratio",hit_ratio));
	m_performanceMetrics->insert(make_pair("Average_Returns",average_returns));


    //Making Performance metrics Report CSV

    ofstream perfOut;
    perfOut.open("../dataFiles/performance_metrics.csv", ios::out);

    perfOut<<"Positive_Returns(%)"<<","<<"Negative_Returns(%)"<<","<<"Positive_Trades"<<","<<"Negative_Trades"<<","<<"Hit_Ratio"<<","<<"Average_Returns(%)"<<endl;

    perfOut<<m_performanceMetrics->at("Positive_Returns")<<","<<m_performanceMetrics->at("Negative_Returns")<<","<<m_performanceMetrics->at("Positive_Trades")<<","<<m_performanceMetrics->at("Negative_Trades")<<","<<m_performanceMetrics->at("Hit_Ratio")<<","<<m_performanceMetrics->at("Average_Returns")<<endl;

    perfOut.flush();
    perfOut.close();

    return m_performanceMetrics;
}