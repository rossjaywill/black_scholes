#ifndef CONSTANTS_H
#define CONSTANTS_H

#define _USE_MATH_DEFINES
#include <math.h>

namespace bsm
{

static constexpr const auto IMPIED_VOL = 0.18;
static constexpr const auto INTEREST   = 0.02;

static constexpr const auto DAY_TO_YEAR  = 365.00;
static constexpr const auto HOUR_TO_DAY  = 24.00;
static constexpr const auto MIN_TO_HOUR  = 60.00;
static constexpr const auto SEC_TO_MIN   = 60.00;
static constexpr const auto MSEC_TO_SEC  = 1000.00;
static constexpr const auto USEC_TO_MSEC = 1000.00;
static constexpr const auto MAX_EXPIRY   = DAY_TO_YEAR * 10.00;

static constexpr const auto MIN_PC     = 0.00;
static constexpr const auto MAX_PC     = 1.00;
static constexpr const auto MIN_PRICE  = 0.00;
static constexpr const auto MAX_PRICE  = 99999.99;

static constexpr const auto DATE_FMT   = "%Y-%m-%d";
// static constexpr const auto EPILSON  = std::numeric_limits<value_type>::epsilon();
static constexpr const auto DP2        = 1E-2;
static constexpr const auto DP3        = 1E-3;

} // bsm

#endif
