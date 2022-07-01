#ifndef GREEKS_H
#define GREEKS_H

#include "black_scholes.h"
#include "constants.h"
#include <cmath>
#include <stdexcept>

namespace bsm
{

// fwd declare executor types
struct CallExecutor;
struct PutExecutor;

template <typename value_type>
class Greeks
{
public:
    explicit Greeks(const OptionValues<value_type> &values,
                    const BlackScholes<value_type> &bsm)
        : values_(values)
        , bsm_(bsm)
    {}

    // Delta declaration
    template <typename Executor>
    constexpr auto delta() { throw std::runtime_error("Cannot derive delta of unknown option type!"); }

    // Delta CALL specialisation
    template <>
    constexpr auto delta<CallExecutor>() /*-> value_type*/ {
        return bsm_.cumulNormalDist(bsm_.d1(values_));
    }

    // Delta PUT specialisation
    template <>
    constexpr auto delta<PutExecutor>() /*-> value_type*/ {
        return bsm_.cumulNormalDist(bsm_.d1(values_)) - 1;
    }

    // Gamma declaration - no specialisations as this reduces to
    // the same formula for both call and puts
    constexpr auto gamma() -> value_type {
        const auto risk = std::exp(-values_.riskFreeInterest_ * values_.timeToExpiry_);
        const auto returns = values_.underlyingPrice_ * values_.volatility_ * std::sqrt(values_.timeToExpiry_);
        return (risk / returns) * standardNormalDensity(bsm_.d1(values_));
    }

    // Theta declaration
    template <typename Executor>
    constexpr auto theta() { throw std::runtime_error("Cannot derive theta of unknown option type!"); }

    // Theta CALL specialisation
    template <>
    constexpr auto theta<CallExecutor>() /*-> value_type*/ {
        const auto spot = values_.underlyingPrice_;
        const auto strike = values_.strikePrice_;
        const auto sqt = std::sqrt(values_.timeToExpiry_);
        const auto discount = std::exp(-values_.riskFreeInterest_ * values_.timeToExpiry_);
        const auto d1 = bsm_.cumulNormalDist(bsm_.d1(values_));

        const auto returns = spot * discount * values_.riskFreeInterest_ * bsm_.cumulNormalDist(d1);
        const auto cost = strike * discount * values_.riskFreeInterest_ * bsm_.cumulNormalDist(d1 - (values_.volatility_ * sqt));
        const auto risk = spot * discount * (values_.volatility_ / (2 * sqt)) * standardNormalDensity(d1);
        return returns - cost - risk;
    }

    // Theta PUT specialisation
    template <>
    constexpr auto theta<PutExecutor>() /*-> value_type*/ {
        const auto spot = values_.underlyingPrice_;
        const auto strike = values_.strikePrice_;
        const auto sqt = std::sqrt(values_.timeToExpiry_);
        const auto discount = std::exp(-values_.riskFreeInterest_ * values_.timeToExpiry_);
        const auto d1 = bsm_.cumulNormalDist(bsm_.d1(values_));

        const auto returns = -spot * discount * values_.riskFreeInterest_ * bsm_.cumulNormalDist(-d1);
        const auto cost = strike * discount * values_.riskFreeInterest_ * bsm_.cumulNormalDist(-d1 + (values_.volatility_ * sqt));
        const auto risk = spot * discount * (values_.volatility_ / (2 * sqt)) * standardNormalDensity(d1);
        return (-returns) + cost - risk;
    }

private:
    // Standard Normal Density
    // i.e. first derivative of Cumulative Normal Distribution
    constexpr inline auto standardNormalDensity(value_type coefficient) {
        return ((1 / std::sqrt(2 * M_PI)) * std::exp(-0.5 * std::pow(coefficient, 2)));
    }

    const OptionValues<value_type> &values_;
    const BlackScholes<value_type> &bsm_;
};

} // bsm

#endif
