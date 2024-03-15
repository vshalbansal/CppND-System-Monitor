#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string timeString;
    long hrs = seconds/3600;
    long minutes = (seconds%3600)/60;
    long secondsRem = seconds%60;
    timeString += to_string(hrs/10)+to_string(hrs%10)+":";
    timeString += to_string(minutes/10)+to_string(minutes%10)+":";
    timeString += to_string(secondsRem/10) + to_string(secondsRem%10);
    return timeString; 
    }