#ifndef HELPERS_H
#define HELPERS_H

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

#include "constants.h"

namespace bsm
{
    // Parse input dates in format: 'YYYY-mm-dd' as time to expiry in years
    template <typename value_type = double>
    auto parseDate(std::string_view date) {
    constexpr auto setToMidnight = [](std::tm &time) {
        time.tm_hour = 0;
        time.tm_min  = 0;
        time.tm_sec  = 0;
    };
    constexpr auto getDaysDelta  = [](value_type diff) {
        return static_cast<value_type>((((diff / SEC_TO_MIN) / MIN_TO_HOUR) / HOUR_TO_DAY));
    };

    const std::time_t now = std::time(nullptr);
    std::tm inTime {};
    std::tm nowLocal = *std::localtime(&now);

    // TODO RJW: Reimplement with 'from_stream' when libc++std 20 updated
    std::stringstream ss(date.data());
    ss >> std::get_time(&inTime, DATE_FMT);
    setToMidnight(inTime);
    setToMidnight(nowLocal);

    const std::time_t expiry  = std::mktime(&inTime);
    const std::time_t current = std::mktime(&nowLocal);

    if (current > expiry) {
        throw std::runtime_error("Option has already expired, please enter a valid date");
    }

    auto diff = std::difftime(expiry, current);
    return getDaysDelta(diff) / DAY_TO_YEAR;
}

}

#endif
