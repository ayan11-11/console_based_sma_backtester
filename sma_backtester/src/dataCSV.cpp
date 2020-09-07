#include "dataCSV.hpp"
#include "utility.hpp"

void dataCSV::cleanup() {
    delete map_adc;
}

void dataCSV::readCSV(char* filePath) {
    
    ifstream csvFile(filePath,ifstream::in);
    string line;
    string temp_str;
    double sstemp_value = 0;

    while(getline(csvFile, line)) {

        replace(line.begin(), line.end(), ',', ' ');

        stringstream ss(line);

        ss >> date;
        
        if(date == "Date") continue;
        
        long int epochdate = get_epoch_time(date);

        ss >> temp_str;
        sscanf((char*)(temp_str).c_str(), "%lf", &sstemp_value);
        open.insert(make_pair(epochdate, sstemp_value));

        ss >> temp_str;
        sscanf((char*)(temp_str).c_str(), "%lf", &sstemp_value);
        high.insert(make_pair(epochdate, sstemp_value));
        
        ss >> temp_str;
        sscanf((char*)(temp_str).c_str(), "%lf", &sstemp_value);
        low.insert(make_pair(epochdate, sstemp_value));
        
        ss >> temp_str;
        sscanf((char*)(temp_str).c_str(), "%lf", &sstemp_value);
        close.insert(make_pair(epochdate, sstemp_value));
        
        ss >> temp_str;
        sscanf((char*)(temp_str).c_str(), "%lf", &sstemp_value);
        volume.insert(make_pair(epochdate, sstemp_value));
        
        ss >> temp_str;
        sscanf((char*)(temp_str).c_str(), "%lf", &sstemp_value);
        adj_close.insert(make_pair(epochdate, sstemp_value));

        //Add the data to the indices
        indices.push_back(epochdate);
    }

    //Enter data into the DataFrame
    data["open"] = open;
    data["high"] = high;
    data["low"] = low;
    data["close"] = close;
    data["volume"] = volume;
    data["adj_close"] = adj_close;

}

long int dataCSV::getAdjCloseMapSize() {
    return (adj_close.size());
}

int dataCSV::getAdjCloseandDateVec(vector<long int>* date, vector<double>* adjclose) {
    
    for(auto &it: adj_close) {
        date->push_back(it.first);
        adjclose->push_back(it.second);
    }
   
    return EXIT_SUCCESS;
}


map<long int, double>* dataCSV::getMap() {
    map_adc = new map<long int, double>(adj_close);

    return map_adc;
}


void dataCSV::writeBuySellCSV(char* filePath, map<long int, int>* m_event, map<long int, double>* m_transaction, map<long int, float>* m_returns) {

    char* buysellreport = filePath;
    
    ofstream csvOut;

    //Making Buy Sell Report CSV
    strcat(buysellreport,"/buy_sell_report.csv");

    csvOut.open(buysellreport, ios::out);
    
    csvOut<<"Date"<<","<<"Trade_Signal"<<","<<"Transaction"<<","<<"Returns(%)"<<endl;

    for(auto itr = m_transaction->begin(); itr != m_transaction->end(); itr++) {
        string date;
        double transaction = 0;
        string trade = "No_Trade";
        float returns = 0;

        date = get_std_time(itr->first);
        transaction = itr->second;

        auto event_itr = m_event->find(itr->first);
        if(event_itr != m_event->end()) {
            if(event_itr->second == 1) {
                trade = "BUY";
            }else if(event_itr->second == 2){
                trade = "SELL";
            }
        }

        auto ret_itr = m_returns->find(itr->first);
        if(ret_itr != m_returns->end()) {
            returns = ret_itr->second;
        }

        csvOut<<date<<","<<trade<<","<<transaction<<","<<returns<<endl;
    }
    csvOut.flush();
    csvOut.close();
}