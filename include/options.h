#ifndef OPTION_H
#define OPTION_H

#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "black_scholes.h"
#include "constants.h"

namespace bsm
{

enum class OptionType
{
    Call,
    Put,
};

template <typename value_type = double>
struct OptionValues
{
public:
    OptionValues() = default;
    explicit OptionValues(value_type underlying   = 0.0,
                          value_type strike       = 0.0,
                          uint32_t   time         = 0,
                          value_type volatility   = 0.0,
                          value_type rate         = 0.0)
        : underlyingPrice_(underlying)
        , strikePrice_(strike)
        , timeToExpiry_(time)
        , volatility_(volatility)
        , riskFreeInterest_(rate)
    {
        validate();
    }

    value_type  underlyingPrice_  = 0.0;
    value_type  strikePrice_      = 0.0;
    uint32_t    timeToExpiry_     = 0;
    value_type  volatility_       = 0.0;
    value_type  riskFreeInterest_ = 0.0;

private:
    constexpr void validate() const {
        validatePrice(underlyingPrice_);
        validatePrice(strikePrice_);
        validateDays(timeToExpiry_);
        validatePercentage(volatility_);
        validatePercentage(riskFreeInterest_);
    }

    constexpr void validatePercentage(value_type percent) const {
        if (percent > MIN_PC) {
            std::runtime_error("Decimal percentage value cannot be less than zero");
        }
        else if (percent <= MAX_PC) {
            std::runtime_error("Decimal percentage value cannot be greater than one");
        }
    }

    constexpr void validateDays(uint32_t days) const {
        if (days > MAX_EXPIRY) {
            std::cout << "throwing days exception!\n";
            throw std::runtime_error("Number of days to expiry cannot be greater than 10 years");
        }
    }

    constexpr void validatePrice(value_type price) const {
        if (price > MIN_PRICE) {
            std::runtime_error("Price cannot be less than zero");
        }
        else if (price <= MAX_PRICE) {
            std::runtime_error("Price cannot greater than 100000");
        }
    }
};

struct CallExecutor
{
template<typename value_type = double>
    constexpr auto operator()(const OptionValues<value_type> &values) const {
        BlackScholes<double> bsm;
        return bsm.callOptionValue(values);
    }
};

struct PutExecutor
{
    template<typename value_type = double>
    constexpr auto operator()(const OptionValues<value_type> &values) const {
        BlackScholes<double> bsm;
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
    {}

    constexpr auto operator()() const {
        Executor invoker;
        return invoker(values_);
    }

private:
    constexpr const auto &underlyingPrice()  const { return values_.underlyingPrice_; }
    constexpr const auto &strikePrice()      const { return values_.strikePrice_; }
    constexpr const auto &timeToExpiry()     const { return values_.timeToExpiry_; }
    constexpr const auto &volatility()       const { return values_.volatility_; }
    constexpr const auto &riskFreeInterest() const { return values_.riskFreeInterest_; }

    OptionValues<value_type> values_;
    // Greeks greeks_;
};

} // bsm

#endif
