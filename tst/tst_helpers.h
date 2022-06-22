#include <chrono>
#include <iomanip>
#include <string>

#include "constants.h"

namespace bsm
{

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

inline std::string getDateOffset(int32_t days) {
    auto current = std::time(nullptr);
    auto local   = *std::localtime(&current);

    auto epoch = std::mktime(&local);
    epoch += days * HOUR_TO_DAY * MIN_TO_HOUR * SEC_TO_MIN;
    auto offset = *std::localtime(&epoch);

    return dateToString(offset);
}

} // bsm
