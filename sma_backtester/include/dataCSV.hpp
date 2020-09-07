/*
*   This class reads the csv file in the data OHLCV format
*/

#ifndef dataCSV_hpp
#define dataCSV_hpp

#include "common_includes.hpp"

using namespace std;

class dataCSV {
    public:
        string date;
        map<long int, double> open;
        map<long int, double> high;
        map<long int, double> low;
        map<long int, double> close;
        map<long int, double> volume;
        map<long int, double> adj_close;

        map<long int, double>* map_adc;

        map<string, map<long int, double>> data;
        vector<long int> indices;

    public:

    void readCSV(char* filePATH);

    long int getAdjCloseMapSize();
    int getAdjCloseandDateVec(vector<long int>* date, vector<double>* adj_close);
    map<long, double>* getMap();

    void writeBuySellCSV(char* filePath, map<long int, int>* m_event, map<long int, double>* m_transaction, map<long int, float>* m_returns);

    void cleanup();
    
};

#endif