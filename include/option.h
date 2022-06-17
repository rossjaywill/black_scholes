#ifndef OPTION_H
#define OPTION_H

#include <cstdint>

#include <fmt/core.h>

namespace bsm
{

enum class CallPutFlag
{
    Call,
    Put
};

template <typename value_type = double>
class Option
{
public:
    Option() = delete;
    explicit Option(CallPutFlag flag         = CallPutFlag::Call,
                     value_type underlying   = 0.0,
                     value_type strike       = 0.0,
                     uint32_t   time         = 0,
                     value_type volatility   = 0.0,
                     value_type rate         = 0.0)
        : optionType_(flag)
        , underlyingPrice_(underlying)
        , strikePrice_(strike)
        , volatility_(volatility)
        , riskFreeInterest_(rate)
        , timeToExpiry_(time)
        {
            fmt::print("Constructed option with:\n"
                "flag: {}\n"
                "underlying: {}\n"
                "strike: {}\n"
                "time: {}\n"
                "volatility: {}\n"
                "rate: {}\n",
                optionType_ == CallPutFlag::Call ? "Call" : "Put",
                underlyingPrice_,
                strikePrice_,
                timeToExpiry_,
                volatility_,
                riskFreeInterest_);
        }

    auto optionType()       { return optionType_; }
    auto underlyingPrice()  { return underlyingPrice_; }
    auto strikePrice()      { return strikePrice_; }
    auto timeToExpiry()     { return timeToExpiry_; }
    auto volatility()       { return volatility_; }
    auto riskFreeInterest() { return riskFreeInterest_; }

private:
    CallPutFlag optionType_       = CallPutFlag::Call;
    value_type  underlyingPrice_  = 0.0;
    value_type  strikePrice_      = 0.0;
    uint32_t    timeToExpiry_     = 0;
    value_type  volatility_       = 0.0;
    value_type  riskFreeInterest_ = 0.0;
};

} // bsm

#endif
