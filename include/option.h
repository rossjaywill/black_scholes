#ifndef OPTION_H
#define OPTION_H

#include "black_scholes.h"

#include <cstdint>
#include <fmt/core.h>

namespace bsm
{

template <typename value_type = double>
struct OptionValues
{
public:
    OptionValues() = default;
    explicit OptionValues(value_type underlying   = 0.0,
                          value_type strike       = 0.0,
                          uint32_t   time         = 0,
                          value_type volatility   = 0.0,
                          value_type rate         = 0.0) noexcept
        : underlyingPrice_(underlying)
        , strikePrice_(strike)
        , timeToExpiry_(time)
        , volatility_(volatility)
        , riskFreeInterest_(rate)
    {}

    value_type  underlyingPrice_  = 0.0;
    value_type  strikePrice_      = 0.0;
    uint32_t    timeToExpiry_     = 0;
    value_type  volatility_       = 0.0;
    value_type  riskFreeInterest_ = 0.0;
};

struct CallExecutor
{
    template<typename value_type = double>
    auto operator()(const OptionValues<value_type> &values) {
        BlackScholes<double> bsm;
        return bsm.callOptionValue(values);
    }
};

struct PutExecutor
{
    template<typename value_type = double>
    auto operator()(const OptionValues<value_type> &values) {
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

    auto operator()() {
        Executor invoker;
        return invoker(values_);
    }

private:
    const auto &underlyingPrice()  const { return values_.underlyingPrice_; }
    const auto &strikePrice()      const { return values_.strikePrice_; }
    const auto &timeToExpiry()     const { return values_.timeToExpiry_; }
    const auto &volatility()       const { return values_.volatility_; }
    const auto &riskFreeInterest() const { return values_.riskFreeInterest_; }

    OptionValues<value_type> values_;
    // Greeks greeks_;
};

} // bsm

#endif
