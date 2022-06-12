#ifndef OPTION_H
#define OPTION_H

#include <cstdint>

namespace black_scholes
{

enum class CallPutFlag
{
    Call,
    Put
};

template <typename RealType = double>
class Option
{
    Option() = delete;
    explicit Option(CallPutFlag flag       = CallPutFlag::Call,
                     RealType   underlying = 0.0,
                     RealType   strike     = 0.0,
                     RealType   volatility = 0.0,
                     RealType   rate       = 0.0,
                     uint32_t   time       = 0)
        : optionType_(flag)
        , underlyingPrice_(underlying)
        , strikePrice_(strike)
        , volatility_(volatility)
        , riskFreeInterest_(rate)
        , timeToExpiry_(time)
        {}

    auto optionType()       { return optionType_; }
    auto underlyingPrice()  { return underlyingPrice_; }
    auto strikePrice()      { return strikePrice_; }
    auto volatility()       { return volatility_; }
    auto riskFreeInterest() { return riskFreeInterest_; }
    auto timeToExpiry()     { return timeToExpiry_; }

private:
    CallPutFlag optionType_       = CallPutFlag::Call;
    RealType    underlyingPrice_  = 0.0;
    RealType    strikePrice_      = 0.0;
    RealType    volatility_       = 0.0;
    RealType    riskFreeInterest_ = 0.0;
    uint32_t    timeToExpiry_     = 0;
};

} // black_scholes

#endif
