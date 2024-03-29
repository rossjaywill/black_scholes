#ifndef OPTION_H
#define OPTION_H

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <fmt/core.h>

#include "black_scholes.h"
#include "constants.h"
#include "greeks.h"

namespace bsm
{

enum class OptionType
{
    Call,
    Put,
    None,
};

template <typename value_type>
struct OptionValues
{
public:
    OptionValues() = delete;
    explicit OptionValues(value_type underlying   = 0.0,
                          value_type strike       = 0.0,
                          value_type time         = 0.0,
                          value_type volatility   = 0.0,
                          value_type rate         = 0.0,
                          value_type yield        = 0.0)
        : underlyingPrice_(underlying)
        , strikePrice_(strike)
        , timeToExpiry_(time)
        , volatility_(volatility)
        , riskFreeInterest_(rate)
        , dividendYield_(yield)
        , sqrtime_(std::sqrt(timeToExpiry_))
        , volatilityPotential_(volatility_ / (2 * sqrtime_))
        , interestDiscount_(std::exp(-riskFreeInterest_ * timeToExpiry_))
        , dividendDiscount_(std::exp(-dividendYield_ * timeToExpiry_))
    {
        validate();
    }

    value_type  underlyingPrice_  = 0.0;
    value_type  strikePrice_      = 0.0;
    value_type  timeToExpiry_     = 0.0;
    value_type  volatility_       = 0.0;
    value_type  riskFreeInterest_ = 0.0;
    value_type  dividendYield_    = 0.0;

    // Memoize common derived terms
    const value_type sqrtime_;              // square root of time to expiry
    const value_type volatilityPotential_;  // potential positive impact of implied volatility
    const value_type interestDiscount_;     // discounting influence of risk free interest rate
    const value_type dividendDiscount_;     // discounting influence of underlying dividend yield

private:
    constexpr void validate() const {
        validatePrice(underlyingPrice_);
        validatePrice(strikePrice_);
        validateExpiryTime(timeToExpiry_);
        validatePercentage(volatility_);
        validatePercentage(riskFreeInterest_);
        validatePercentage(dividendYield_);
    }

    constexpr void validatePercentage(const value_type percent) const {
        if (percent < MIN_PC) {
            throw std::runtime_error("Decimal percentage value cannot be less than zero");
        }
        else if (percent >= MAX_PC) {
            throw std::runtime_error("Decimal percentage value cannot be greater than one");
        }
    }

    constexpr void validateExpiryTime(const value_type time) const {
        if (time * DAY_TO_YEAR > MAX_EXPIRY) {
            throw std::runtime_error("Time to expiry cannot be greater than 10 years");
        }
    }

    constexpr void validatePrice(const value_type price) const {
        if (price < MIN_PRICE) {
            throw std::runtime_error("Price cannot be less than zero");
        }
        else if (price >= MAX_PRICE) {
            throw std::runtime_error("Price cannot greater than 100000");
        }
    }
};

struct CallExecutor
{
    template<typename value_type>
    constexpr auto operator()(const BlackScholes<value_type> &bsm,
                                     const OptionValues<value_type> &values) const {
        return bsm.callOptionValue(values);
    }
};

struct PutExecutor
{
    template<typename value_type>
    constexpr auto operator()(const BlackScholes<value_type> &bsm,
                                     const OptionValues<value_type> &values) const {
        return bsm.putOptionValue(values);
    }
};

template <typename Executor = CallExecutor,
          typename value_type = double>
class Option
{
public:
    Option() = delete;
    explicit Option(OptionValues<value_type> &&values) noexcept
        : values_(std::move(values))
        , bsm_()
        , greeks_(values_, bsm_)
    {}

    auto operator=(const Option &rhs) -> Option& {
        if (this != *rhs) {
            this->values_ = rhs.values_;
        }
        return *this;
    }
    auto operator=(Option &&rhs) noexcept -> Option& {
        if (this != *rhs) {
            this->values_ = std::move(rhs.values_);
        }
        return *this;
    }

    constexpr auto operator()() const {
        return Executor()(bsm_, values_);
    }

    // Base values
    constexpr auto underlyingPrice()  const -> value_type { return values_.underlyingPrice_; }
    constexpr auto strikePrice()      const -> value_type { return values_.strikePrice_; }
    constexpr auto timeToExpiry()     const -> value_type { return values_.timeToExpiry_; }
    constexpr auto volatility()       const -> value_type { return values_.volatility_; }
    constexpr auto riskFreeInterest() const -> value_type { return values_.riskFreeInterest_; }
    constexpr auto dividendYield()    const -> value_type { return values_.dividendYield_; }

    // Greeks
    constexpr auto delta() -> value_type { return greeks_.template delta<Executor>(); }
    constexpr auto gamma() -> value_type { return greeks_.gamma(); }
    constexpr auto theta() -> value_type { return greeks_.template theta<Executor>(); }
    constexpr auto vega()  -> value_type { return greeks_.vega(); }
    constexpr auto rho()   -> value_type { return greeks_.template rho<Executor>(); }

    void printGreeks(const bool singleLine = false) {
        std::string_view endl = "\n";
        if (singleLine) {
            endl = ", ";
        }
        fmt::print("Δ: {:.3f}{}Γ: {:.3f}{}Θ: {:.3f}{}ν: {:.3f}{}ρ: {:.3f}\n",
                   delta(), endl, gamma(), endl, theta(), endl, vega(), endl, rho()
        );
    }

private:
    OptionValues<value_type> values_;
    BlackScholes<value_type> bsm_;
    Greeks<value_type> greeks_;
};

} // bsm

#endif
