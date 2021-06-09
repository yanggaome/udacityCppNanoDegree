#include <string>

#include "format.h"

using std::string;

// YG DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string timeFormat(long t) {
    return (t < 10) ?  "0"+std::to_string(t) : std::to_string(t);
}
string Format::ElapsedTime(long seconds) {
    long mins = seconds / 60; // quotient
    long hours = mins / 60; // quotient

    mins = mins % 60; // remainder
    long secs = seconds % 60; // remainder

    return timeFormat(hours) + ":" + timeFormat(mins) + ":" + timeFormat(secs);
}