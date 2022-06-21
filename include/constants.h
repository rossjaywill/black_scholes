#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace bsm
{

static constexpr const auto IMPIED_VOL = 0.18;
static constexpr const auto INTEREST   = 0.02;

static constexpr const auto YEAR       = 365;
static constexpr const auto MAX_EXPIRY = YEAR * 10;

static constexpr const auto MIN_PC     = 0;
static constexpr const auto MAX_PC     = 1;
static constexpr const auto MIN_PRICE  = 0;
static constexpr const auto MAX_PRICE  = 99999.99;

} // bsm

#endif
