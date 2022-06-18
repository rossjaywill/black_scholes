#ifndef OPTION_H
#define OPTION_H

#include <cstdint>
#include <fmt/core.h>

namespace bsm
{

struct CallOption
{
    void operator() () {
        fmt::print("CallOption invoker called!");
    }
};

struct PutOption
{
    void operator()() {
        fmt::print("PutOption invoker called!");
    }
};

template <typename CallPut = CallOption, typename value_type = double>
class Option
{
public:
    Option() = delete;
    explicit Option(value_type underlying   = 0.0,
                    value_type strike       = 0.0,
                    uint32_t   time         = 0,
                    value_type volatility   = 0.0,
                    value_type rate         = 0.0)
        : underlyingPrice_(underlying)
        , strikePrice_(strike)
        , volatility_(volatility)
        , riskFreeInterest_(rate)
        , timeToExpiry_(time)
    {
        fmt::print("Constructed option with:\n"
            "underlying: {}\n"
            "strike: {}\n"
            "time: {}\n"
            "volatility: {}\n"
            "rate: {}\n",
            underlyingPrice_,
            strikePrice_,
            timeToExpiry_,
            volatility_,
            riskFreeInterest_);
    }

    void operator()() {
        CallPut invoker;
        invoker();
    }

    const auto &underlyingPrice()  const { return underlyingPrice_; }
    const auto &strikePrice()      const { return strikePrice_; }
    const auto &timeToExpiry()     const { return timeToExpiry_; }
    const auto &volatility()       const { return volatility_; }
    const auto &riskFreeInterest() const { return riskFreeInterest_; }

private:
    value_type  underlyingPrice_  = 0.0;
    value_type  strikePrice_      = 0.0;
    uint32_t    timeToExpiry_     = 0;
    value_type  volatility_       = 0.0;
    value_type  riskFreeInterest_ = 0.0;
};

} // bsm

#endif
