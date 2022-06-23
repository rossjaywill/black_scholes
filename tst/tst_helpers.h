#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

#include "constants.h"

namespace bsm
{
using value_type = double;
static constexpr const auto EPILSON  = std::numeric_limits<value_type>::epsilon();

static bool valueEquals(value_type lhs, value_type rhs) {
    std::cout << "lhs: " << lhs << " rhs: " << rhs << "\n";
    std::cout << "abs: " << std::fabs(lhs - rhs) << "\n";
    std::cout << "epsilon: " << EPILSON << "\n";
    std::cout << "epsilon * max: " << EPILSON * std::max(lhs, rhs) << "\n";

    // if (std::fabs(lhs - rhs) == 0) {
    //     return true;
    // }
    return (std::fabs(lhs - rhs) <= EPILSON * std::max({ 1.0, lhs, rhs }));
}

inline std::string dateToString(std::tm &time) {
    std::ostringstream oss;
    oss << std::put_time(&time, DATE_FMT);
    return oss.str();
}

inline std::string getCurrentDate() {
    auto current = std::time(nullptr);
    auto local   = *std::localtime(&current);

    return dateToString(local);
}

inline std::string getDateOffset(value_type days) {
    auto current = std::time(nullptr);
    auto local   = *std::localtime(&current);

    auto epoch = std::mktime(&local);
    epoch += static_cast<std::time_t>(days * HOUR_TO_DAY * MIN_TO_HOUR * SEC_TO_MIN);
    auto offset = *std::localtime(&epoch);

    return dateToString(offset);
}

} // bsm
