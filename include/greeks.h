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

    // Delta specialisations
    template <>
    constexpr auto delta<CallExecutor>() {
        return bsm_.cumulNormalDist(bsm_.d1(values_));
    }

    template <>
    constexpr auto delta<PutExecutor>() {
        return bsm_.cumulNormalDist(bsm_.d1(values_)) - 1;
    }

    // Gamma declaration - no specialisations as this reduces to
    // the same formula for both call and puts
    constexpr auto gamma() {
        const auto risk = std::exp(-values_.riskFreeInterest_ * values_.timeToExpiry_);
        const auto returns = values_.underlyingPrice_ * values_.volatility_ * std::sqrt(values_.timeToExpiry_);
        return (risk / returns) * standardNormalDensity(bsm_.d1(values_));
    }

private:
    // Standard Normal Density
    // i.e. first derivative of Cumulative Normal Distribution
    constexpr inline auto standardNormalDensity(value_type coefficient) {
        return (1 / std::sqrt(2 * M_PI) * std::exp(std::pow(-coefficient, 2) / 2));
    }

    const OptionValues<value_type> &values_;
    const BlackScholes<value_type> &bsm_;
};

} // bsm

#endif
