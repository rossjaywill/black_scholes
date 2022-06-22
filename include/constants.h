#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace bsm
{

static constexpr const auto IMPIED_VOL = 0.18;
static constexpr const auto INTEREST   = 0.02;

static constexpr const auto DAY_TO_YEAR  = 365;
static constexpr const auto HOUR_TO_DAY  = 24;
static constexpr const auto MIN_TO_HOUR  = 60;
static constexpr const auto SEC_TO_MIN   = 60;
static constexpr const auto MSEC_TO_SEC  = 1000;
static constexpr const auto USEC_TO_MSEC = 1000;
static constexpr const auto MAX_EXPIRY   = DAY_TO_YEAR * 10;

static constexpr const auto MIN_PC     = 0;
static constexpr const auto MAX_PC     = 1;
static constexpr const auto MIN_PRICE  = 0;
static constexpr const auto MAX_PRICE  = 99999.99;

static constexpr const auto DATE_FMT   = "%Y-%m-%d";

} // bsm

#endif
