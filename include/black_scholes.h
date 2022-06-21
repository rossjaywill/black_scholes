#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include <cmath>

namespace bsm
{

template<typename value_type>
struct OptionValues;

template<typename value_type = double>
class BlackScholes
{
public:
    BlackScholes() = default;

    constexpr auto callOptionValue(const OptionValues<value_type> &values) const {
        auto dOne = d1(values);
        auto returnProb = returnsProbability(values.underlyingPrice_, dOne);
        auto costProb = costProbability(values, d2(dOne, values));
        return returnProb - costProb;
    }

    constexpr auto putOptionValue(const OptionValues<value_type> &values) const {
        auto dOne = d1(values);
        auto costProb = costProbability(values, -d2(dOne, values));
        auto returnProb = returnsProbability(values.underlyingPrice_, -dOne);
        return costProb - returnProb;
    }

private:
    constexpr auto cumulNormalDist(value_type coefficient) const {
        return static_cast<value_type>(0.5) * std::erfc(-coefficient * (1 / std::sqrt(2)));
    }

    constexpr auto returnsProbability(value_type underlying, value_type ratio) const {
        return underlying * cumulNormalDist(ratio);
    }

    constexpr auto costProbability(const OptionValues<value_type> &values, value_type ratio) const {
        return values.strikePrice_ * std::exp(-values.riskFreeInterest_ * values.timeToExpiry_) * cumulNormalDist(ratio);
    }

    constexpr auto d1(const OptionValues<value_type> &values) const {
        auto ratio = std::log(values.underlyingPrice_ / values.strikePrice_);
        auto riskFreeTime = values.riskFreeInterest_ + ((std::pow(values.volatility_, 2) / 2) * values.timeToExpiry_);
        auto time = values.volatility_ * (std::sqrt(values.timeToExpiry_));
        return (ratio + riskFreeTime) / time;
    }

    constexpr auto d2(value_type d1, const OptionValues<value_type> &values) const {
        auto time = values.volatility_ * (std::sqrt(values.timeToExpiry_));
        return d1 - time;
    }
};

} // bsm

#endif
