#include "black_scholes.h"
#include "constants.h"
#include "options.h"
#include "tst_helpers.h"

#include "catch2/catch.hpp"

using namespace bsm;
using Catch::Matchers::Contains;

TEST_CASE("In the money calls, out of the money puts", "[itm]")
{
    OptionValues<value_type> input { 100.00, 95.00, 1, 0.18, 0.05 };

    SECTION("ITM one year expiry CALL")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 12.692));
    }

    SECTION("OTM one year expiry PUT")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 3.059));
    }

    SECTION("ITM half year expiry CALL")
    {
        input.timeToExpiry_ = 0.5;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 9.339));
    }

    SECTION("OTM half year expiry PUT")
    {
        input.timeToExpiry_ = 0.5;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 1.994));
    }

    SECTION("ITM quarter year expiry CALL")
    {
        input.timeToExpiry_ = 0.25;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 7.221));
    }

    SECTION("OTM quarter year expiry PUT")
    {
        input.timeToExpiry_ = 0.25;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 1.041));
    }

    SECTION("ITM one year expiry high volatility CALL")
    {
        input.volatility_ = 0.6;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 27.572));
    }

    SECTION("OTM one year expiry high volatility PUT")
    {
        input.volatility_ = 0.6;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 17.939));
    }

    SECTION("ITM one year expiry low interest CALL")
    {
        input.riskFreeInterest_ = 0.01;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 10.329));
    }

    SECTION("OTM one year expiry low interest PUT")
    {
        input.riskFreeInterest_ = 0.01;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 4.384));
    }
}

TEST_CASE("Out of the money calls, in the money puts", "[otm]")
{
    OptionValues<value_type> input { 20.15, 35.20, 1, 0.25, 0.03 };

    SECTION("ITM one year expiry CALL")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 0.041));
    }

    SECTION("OTM one year expiry PUT")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 14.051));
    }

    SECTION("ITM half year expiry CALL")
    {
        input.timeToExpiry_ = 0.5;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 0.001));
    }

    SECTION("OTM half year expiry PUT")
    {
        input.timeToExpiry_ = 0.5;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 14.527));
    }

    SECTION("ITM quarter year expiry CALL")
    {
        input.timeToExpiry_ = 0.25;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 0));
    }

    SECTION("OTM quarter year expiry PUT")
    {
        input.timeToExpiry_ = 0.25;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 14.787));
    }

    SECTION("ITM one year expiry high volatility CALL")
    {
        input.volatility_ = 0.6;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 1.596));
    }

    SECTION("OTM one year expiry high volatility PUT")
    {
        input.volatility_ = 0.6;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 15.606));
    }

    SECTION("ITM one year expiry low interest CALL")
    {
        input.riskFreeInterest_ = 0.01;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 0.033));
    }

    SECTION("OTM one year expiry low interest PUT")
    {
        input.riskFreeInterest_ = 0.01;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(valueEquals(option(), 14.733));
    }
}
