#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include <cmath>

namespace bsm
{

// fwd declare option value type
template<typename value_type>
struct OptionValues;

template<typename value_type>
class BlackScholes
{
public:
    BlackScholes() = default;

    inline constexpr auto callOptionValue(const OptionValues<value_type> &values) const {
        const auto dOne = d1(values);
        const auto returnProb = returnsProbability(values.underlyingPrice_, dOne);
        const auto costProb = costProbability(values, d2(dOne, values));
        return returnProb - costProb;
    }

    inline constexpr auto putOptionValue(const OptionValues<value_type> &values) const {
        const auto dOne = d1(values);
        const auto costProb = costProbability(values, -d2(dOne, values));
        const auto returnProb = returnsProbability(values.underlyingPrice_, -dOne);
        return costProb - returnProb;
    }

    inline constexpr auto cumulNormalDist(const value_type coefficient) const {
        return static_cast<value_type>(0.5) * std::erfc(-coefficient * (1 / std::sqrt(2)));
    }

    inline constexpr auto d1(const OptionValues<value_type> &values) const {
        const auto ratio = std::log(values.underlyingPrice_ / values.strikePrice_);
        const auto volTime = ((std::pow(values.volatility_, 2) / 2) * values.timeToExpiry_);
        const auto riskFreeTime = values.riskFreeInterest_ + volTime;
        const auto time = values.volatility_ * (std::sqrt(values.timeToExpiry_));
        return (ratio + riskFreeTime) / time;
    }

    inline constexpr auto d2(const value_type d1, const OptionValues<value_type> &values) const {
        const auto time = values.volatility_ * (std::sqrt(values.timeToExpiry_));
        return d1 - time;
    }

private:
    inline constexpr auto returnsProbability(value_type underlying, const value_type ratio) const {
        return underlying * cumulNormalDist(ratio);
    }

    inline constexpr auto costProbability(const OptionValues<value_type> &values, const value_type ratio) const {
        const auto riskFreeReturns = std::exp(-values.riskFreeInterest_ * values.timeToExpiry_);
        return values.strikePrice_ * riskFreeReturns * cumulNormalDist(ratio);
    }
};

} // bsm

#endif
