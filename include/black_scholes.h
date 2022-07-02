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
        const auto returns = returnsProbability(values.underlyingPrice_ * 1, dOne); // e^(-rf*t) assume divid rf = 0 i.e. this term = 1
        const auto cost = costProbability(values, d2(dOne, values));
        const auto value = returns - cost;
        return (value > 0) ? value : 0.00;
    }

    inline constexpr auto putOptionValue(const OptionValues<value_type> &values) const {
        const auto dOne = d1(values);
        const auto cost = costProbability(values, -d2(dOne, values));
        const auto returns = returnsProbability(values.underlyingPrice_ * 1, -dOne); // e^(-rf*t) assume divid rf = 0 i.e. this term = 1
        const auto value = cost - returns;
        return (value > 0) ? value : 0.00;
    }

    inline constexpr auto d1(const OptionValues<value_type> &values) const {
        const auto ratio = std::log(values.underlyingPrice_ / values.strikePrice_);
        const auto volVariance = std::pow(values.volatility_, 2) / 2;
        const auto discountedTime = values.riskFreeInterest_ - values.dividendYield_ + volVariance; // minus divid, assumed 0 here.
        const auto probability = (ratio + discountedTime) * values.timeToExpiry_;
        const auto time = values.volatility_ * (std::sqrt(values.timeToExpiry_));
        return (1 / time) * probability;
    }

    inline constexpr auto d2(const value_type d1, const OptionValues<value_type> &values) const {
        const auto time = values.volatility_ * (std::sqrt(values.timeToExpiry_));
        return d1 - time;
    }

    inline constexpr auto cumulNormalDist(const value_type coefficient) const {
        return static_cast<value_type>(0.5) * std::erfc(-coefficient * (1 / std::sqrt(2)));
    }

private:
    inline constexpr auto returnsProbability(const value_type underlying, const value_type ratio) const {
        return underlying * cumulNormalDist(ratio);
    }

    inline constexpr auto costProbability(const OptionValues<value_type> &values, const value_type ratio) const {
        const auto riskFreeReturns = std::exp(-values.riskFreeInterest_ * values.timeToExpiry_);
        return values.strikePrice_ * riskFreeReturns * cumulNormalDist(ratio);
    }
};

} // bsm

#endif
