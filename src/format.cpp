#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{
    auto minutes = seconds / 60;
    auto hours = minutes / 60;
    auto secondsRemaining = seconds % 60;
    auto minutesRemaining = minutes % 60;

    return std::to_string(hours) + ":" + std::to_string(minutesRemaining) + ":" + std::to_string(secondsRemaining);
}