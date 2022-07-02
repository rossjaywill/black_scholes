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
        , spot_(values_.underlyingPrice_) // ref. aliased for readability below
        , strike_(values_.strikePrice_)   // ref. aliased for readability below

        // Memoize common derived terms as members below, for performance and
        // readability in calculations below.
        , d1_(bsm_.d1(values_))
        , probabilityExercised_(bsm_.cumulNormalDist(d1_))
        , probabilityExpires_(bsm_.cumulNormalDist(-d1_))
        , probabilityReturns_(bsm_.cumulNormalDist(d1_ - (values_.volatility_ * values_.sqrtime_)))
        , probabilityCost_(bsm_.cumulNormalDist(-d1_ + (values_.volatility_ * values_.sqrtime_)))
        , nd1_(standardNormalDensity(d1_))
    {}

    // Delta declaration
    template <typename Executor>
    constexpr auto delta() { throw std::runtime_error("Cannot derive delta of unknown option type!"); }

    // Delta CALL specialisation
    template <>
    constexpr auto delta<CallExecutor>() {
        return values_.dividendDiscount_ * probabilityExercised_;
    }

    // Delta PUT specialisation
    template <>
    constexpr auto delta<PutExecutor>() {
        return -values_.dividendDiscount_ * probabilityExpires_;
    }

    // Gamma declaration - no specialisations as this reduces to
    // the same formula for both call and puts
    constexpr auto gamma() -> value_type {
        const auto returns = strike_ * values_.volatility_ * values_.sqrtime_;
        return (values_.dividendDiscount_ / returns) * nd1_;
    }

    // Theta declaration
    template <typename Executor>
    constexpr auto theta() { throw std::runtime_error("Cannot derive theta of unknown option type!"); }

    // Theta CALL specialisation
    template <>
    constexpr auto theta<CallExecutor>() {
        const auto returns = spot_ * values_.dividendDiscount_ * values_.dividendYield_ * probabilityExercised_;
        const auto cost = strike_ * values_.interestDiscount_ * values_.riskFreeInterest_ * probabilityReturns_;
        const auto dividend = spot_ * values_.dividendDiscount_ * values_.volatilityPotential_ * nd1_;

        return returns - cost - dividend;
    }

    // Theta PUT specialisation
    template <>
    constexpr auto theta<PutExecutor>() {
        const auto returns = spot_ * values_.dividendDiscount_ * values_.dividendYield_ * probabilityExpires_;
        const auto cost = strike_ * values_.interestDiscount_ * values_.riskFreeInterest_ * probabilityCost_;
        const auto dividend = spot_ * values_.dividendDiscount_ * values_.volatilityPotential_ * nd1_;

        return (-returns) + cost - dividend;
    }

    // Vega declaration - no specialisations as this reduces to
    // the same formula for both call and puts
    constexpr auto vega() -> value_type {
        const auto returns = spot_ * values_.dividendDiscount_;
        return returns * values_.sqrtime_ * nd1_;
    }

    // Rho declaration
    template <typename Executor>
    constexpr auto rho() { throw std::runtime_error("Cannot derive rho of unknown option type!"); }

    // Rho CALL specialisation
    template <>
    constexpr auto rho<CallExecutor>() {
        const auto cost = strike_ * values_.interestDiscount_;
        return cost * probabilityReturns_;
    }

    // Rho PUT specialisation
    template <>
    constexpr auto rho<PutExecutor>() {
        const auto cost = (-strike_) * values_.interestDiscount_;
        return cost * probabilityCost_;
    }

private:
    // Standard Normal Density
    // i.e. first derivative of Cumulative Normal Distribution
    constexpr inline auto standardNormalDensity(value_type coefficient) {
        return ((1 / std::sqrt(2 * M_PI)) * std::exp(-0.5 * std::pow(coefficient, 2)));
    }

    // Underlying Inputs
    const OptionValues<value_type> &values_;
    const BlackScholes<value_type> &bsm_;
    // Alias input values for readability in calculations
    const value_type &spot_;
    const value_type &strike_;

    // Memoize common derived terms
    const value_type d1_;                   // current position: ratio of current spot price to strike price
    const value_type probabilityExercised_; // probably of exercising at current spot price
    const value_type probabilityExpires_;   // probably of not exercising at current spot price
    const value_type probabilityReturns_;   // probably of returns at current position, after accounting for interest and yield
    const value_type probabilityCost_;      // probably of costs at current position, after accounting for interest and yield
    const value_type nd1_;                  // standard normal density of d1
};

} // bsm

#endif
