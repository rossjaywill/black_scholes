#ifndef OPTION_H
#define OPTION_H

#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "black_scholes.h"
#include "constants.h"
#include "greeks.h"

namespace bsm
{

enum class OptionType
{
    Call,
    Put,
};

template <typename value_type>
struct OptionValues
{
public:
    OptionValues() = default;
    explicit OptionValues(value_type underlying   = 0.0,
                          value_type strike       = 0.0,
                          value_type time         = 0.0,
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
    value_type  timeToExpiry_     = 0.0;
    value_type  volatility_       = 0.0;
    value_type  riskFreeInterest_ = 0.0;

private:
    inline constexpr void validate() const {
        validatePrice(underlyingPrice_);
        validatePrice(strikePrice_);
        validateExpiryTime(timeToExpiry_);
        validatePercentage(volatility_);
        validatePercentage(riskFreeInterest_);
    }

    inline constexpr void validatePercentage(const value_type percent) const {
        if (percent < MIN_PC) {
            throw std::runtime_error("Decimal percentage value cannot be less than zero");
        }
        else if (percent >= MAX_PC) {
            throw std::runtime_error("Decimal percentage value cannot be greater than one");
        }
    }

    inline constexpr void validateExpiryTime(const value_type time) const {
        if (time * DAY_TO_YEAR > MAX_EXPIRY) {
            throw std::runtime_error("Time to expiry cannot be greater than 10 years");
        }
    }

    inline constexpr void validatePrice(const value_type price) const {
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
    inline constexpr auto operator()(const BlackScholes<value_type> &bsm,
                                     const OptionValues<value_type> &values) const {
        return bsm.callOptionValue(values);
    }
};

struct PutExecutor
{
    template<typename value_type>
    inline constexpr auto operator()(const BlackScholes<value_type> &bsm,
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
    Option(const Option &rhs)
        : values_(rhs.values_)
        , bsm_(rhs.bsm_)
        , greeks_(rhs.greeks_)
    {}
    Option(Option &&rhs) noexcept
        : values_(std::move(rhs.values_))
        , bsm_(std::move(rhs.bsm_))
        , greeks_(std::move(rhs.greeks_))
    {}
    ~Option() {}

    void operator=(const Option &rhs) { this->values_ = rhs.values_; }
    void operator=(Option &&rhs) { this->values_ = std::move(rhs.values_); }

    inline constexpr auto operator()() const {
        Executor invoker;
        return invoker(bsm_, values_);
    }

    // Base values
    inline constexpr const auto &underlyingPrice()  const { return values_.underlyingPrice_; }
    inline constexpr const auto &strikePrice()      const { return values_.strikePrice_; }
    inline constexpr const auto &timeToExpiry()     const { return values_.timeToExpiry_; }
    inline constexpr const auto &volatility()       const { return values_.volatility_; }
    inline constexpr const auto &riskFreeInterest() const { return values_.riskFreeInterest_; }

    // Greeks
    inline constexpr auto delta() { return greeks_.template delta<Executor>(); }
    inline constexpr auto gamma() { return greeks_.gamma(); }
    // value_type theta() { return greeks_.template theta<Executor>(); }
    // value_type vega()  { return greeks_.template vega<Executor>(); }
    // value_type rho()   { return greeks_.template rho<Executor>(); }

private:
    OptionValues<value_type> values_;
    BlackScholes<value_type> bsm_;
    Greeks<value_type> greeks_;
};

} // bsm

#endif
