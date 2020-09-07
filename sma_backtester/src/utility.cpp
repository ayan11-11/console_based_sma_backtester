#include "utility.hpp"

//Find epoch time from normal DD-MM-YYYY
long int get_epoch_time(string date) {
    struct tm t = {0};
    time_t timeSinceEpoch;
    
    istringstream ss(date);
    
    if(ss >> get_time(&t, "%d-%m-%Y")) {
        timeSinceEpoch = mktime(&t);
    }

    // cout << "timeSinceEpoch: " << timeSinceEpoch << endl;;
    
    return long(timeSinceEpoch);
}


//Find normal time DD-MM-YYYY from epoch time
// Find normal time YYYY-MM-DD from epoch time
string get_std_time(long int epochtime) {
    time_t t = epochtime;
    char string[80];
    strftime(string, 80, "%d-%m-%Y", gmtime(&t));
    return string;
}