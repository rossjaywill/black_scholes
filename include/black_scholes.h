#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include <cmath>
#include <cstdint>
#include <vector>

namespace bsm
{

template<typename value_type = double>
class BlackScholes
{
public:
    BlackScholes() = default;
    auto valueAtExpiry();

    value_type callOption(value_type underlying, value_type strike, value_type interest, value_type volatility, uint32_t expiry) {
        auto dOne = d1(underlying, strike, interest, volatility, expiry);
        auto returnProb = returnsProbability(underlying, dOne);
        auto costProb = costProbability(strike, interest, expiry, d2(dOne, volatility, expiry));
        return returnProb - costProb;
    }

    value_type putOption(value_type underlying, value_type strike, value_type interest, value_type volatility, uint32_t expiry) {
        auto dOne = d1(underlying, strike, interest, volatility, expiry);
        auto costProb = costProbability(strike, interest, expiry, -d2(dOne, volatility, expiry));
        auto returnProb = returnsProbability(underlying, -dOne);
        return costProb - returnProb;
    }

private:
    value_type cumulNormalDist(value_type coefficient) {
        return static_cast<value_type>(0.5) * std::erfc(-coefficient * (1 / std::sqrt(2)));
    }

    value_type returnsProbability(value_type underlying, value_type ratio) {
        return underlying * cumulNormalDist(ratio);
    }

    value_type costProbability(value_type strike, value_type interest, uint32_t expiry, value_type ratio) {
        return strike * std::exp(-interest * expiry) * cumulNormalDist(ratio);
    }

    value_type d1(value_type underlying, value_type strike, value_type interest, value_type volatility, uint32_t expiry) {
        auto ratio = std::log(underlying / strike);
        auto riskFreeTime = interest + ((std::pow(volatility, 2) / 2) * expiry);
        auto time = volatility * (std::sqrt(expiry));
        return (ratio + riskFreeTime) / time;
    }

    value_type d2(value_type d1, value_type volatility, uint32_t expiry) {
        auto time = volatility * (std::sqrt(expiry));
        return d1 - time;
    }
};

} // bsm

#endif
