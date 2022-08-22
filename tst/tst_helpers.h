#ifndef TST_HELPERS_H
#define TST_HELPERS_H

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

#include "constants.h"

namespace bsm
{
using value_type = double;

// Verify floating point values are equal to [precision] decimal places.
static constexpr auto compareFloat(value_type lhs,
                                          value_type rhs,
                                          value_type precision = DP2) -> bool {
    return (std::fabs(lhs - rhs) <= precision);
}

// Parse time struct to YYYY-mm-dd format
static auto dateToString(std::tm &time) -> std::string {
    std::ostringstream oss;
    oss << std::put_time(&time, DATE_FMT);
    return oss.str();
}

// Get current date as YYYY-mm-dd format
[[maybe_unused]] static auto getCurrentDate() -> std::string {
    auto current = std::time(nullptr);
    auto local   = *std::localtime(&current);

    return dateToString(local);
}

// Get date as offset in days from current date
[[maybe_unused]] static auto getDateOffset(value_type days) -> std::string {
    auto current = std::time(nullptr);
    auto local   = *std::localtime(&current);

    auto epoch = std::mktime(&local);
    epoch += static_cast<std::time_t>(days * HOUR_TO_DAY * MIN_TO_HOUR * SEC_TO_MIN);
    auto offset = *std::localtime(&epoch);

    return dateToString(offset);
}

} // bsm

#endif
