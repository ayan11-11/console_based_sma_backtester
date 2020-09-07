#include "common_includes.hpp"
#include "dataCSV.hpp"
#include "sma_calculate.hpp"
#include "buy_sell_event.hpp"

int main(int argc, char* argv[]) {
	
    if(argc < 5) {
        cout << "Please run the following command in the src directory: "<< endl << "\"./sma_backtest ../dataFiles/sma_data.csv 5 26 ../dataFiles\"" << endl;
        cout << "The pogram takes csv file as input file, which has to be placed in the dataFiles folder in date OHLCV. " << endl;
        cout << "The output reports are generated and placed in the dataFiles folder in csv format."<< endl << "Thank You, Have Fun." << endl;
        exit(1);
    }

    vector<long int> date_vec;
    vector<double> adj_close_vec;

    //Objects
	dataCSV csv;
    smaCalc smac;
    bsEvent bsevent;

    csv.readCSV(argv[1]);
    date_vec.reserve(csv.getAdjCloseMapSize());
    adj_close_vec.reserve(csv.getAdjCloseMapSize());
    csv.getAdjCloseandDateVec(&date_vec, &adj_close_vec);
    map<long int,double>* adc_map = const_cast<map<long int,double>*> (csv.getMap());

    int first_period = atoi(argv[2]);
    map<long int,double>* first_period_result = const_cast<map<long int,double>*> (smac.calculate_sma(&date_vec, &adj_close_vec, first_period, 1));

    int second_period = atoi(argv[3]);
    map<long int,double>* second_period_result = const_cast<map<long int,double>*> (smac.calculate_sma(&date_vec, &adj_close_vec, second_period, 2));

    map<long int, int>* m_event = const_cast<map<long int,int>*> (bsevent.getBSEvent(first_period_result, second_period_result));
    map<long int, double>* m_transaction = const_cast<map<long int,double>*> (bsevent.getTransaction(&date_vec, adc_map));
    map<long int, float>* m_returns = const_cast<map<long int,float>*> (bsevent.getReturns());
    map<string, float>* m_perfMetrics = const_cast<map<string,float>*> (bsevent.getPerformanceMetrics());

    csv.writeBuySellCSV(argv[4], m_event, m_transaction, m_returns);
    csv.cleanup();


    return EXIT_SUCCESS;
}
