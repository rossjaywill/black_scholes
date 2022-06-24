#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

#include "constants.h"

namespace bsm
{
using value_type = double;

// Verify floating point values are equal to 3 decimal places.
static bool valueEquals(value_type lhs, value_type rhs) {
    return (std::fabs(lhs - rhs) <= DP3);
}

// Parse time struct to YYYY-mm-dd format
inline std::string dateToString(std::tm &time) {
    std::ostringstream oss;
    oss << std::put_time(&time, DATE_FMT);
    return oss.str();
}

// Get current date as YYYY-mm-dd format
inline std::string getCurrentDate() {
    auto current = std::time(nullptr);
    auto local   = *std::localtime(&current);

    return dateToString(local);
}

// Get date as offset in days from current date
inline std::string getDateOffset(value_type days) {
    auto current = std::time(nullptr);
    auto local   = *std::localtime(&current);

    auto epoch = std::mktime(&local);
    epoch += static_cast<std::time_t>(days * HOUR_TO_DAY * MIN_TO_HOUR * SEC_TO_MIN);
    auto offset = *std::localtime(&epoch);

    return dateToString(offset);
}

} // bsm
