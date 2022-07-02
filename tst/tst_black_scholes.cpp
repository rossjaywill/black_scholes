#include "black_scholes.h"
#include "constants.h"
#include "options.h"
#include "tst_helpers.h"

#include "catch2/catch.hpp"

using namespace bsm;

TEST_CASE("In the money calls, out of the money puts", "[itm]")
{
    OptionValues<value_type> input { 100.00, 95.00, 1, 0.18, 0.05 };

    SECTION("ITM one year expiry CALL")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 12.69));
    }

    SECTION("OTM one year expiry PUT")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 3.06));
    }

    SECTION("ITM half year expiry CALL")
    {
        input.timeToExpiry_ = 0.5;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 9.32));
    }

    SECTION("OTM half year expiry PUT")
    {
        input.timeToExpiry_ = 0.5;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 1.98));
    }

    SECTION("ITM quarter year expiry CALL")
    {
        input.timeToExpiry_ = 0.25;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 7.12));
    }

    SECTION("OTM quarter year expiry PUT")
    {
        input.timeToExpiry_ = 0.25;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 0.934));
    }

    SECTION("ITM one year expiry high volatility CALL")
    {
        input.volatility_ = 0.6;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 27.57));
    }

    SECTION("OTM one year expiry high volatility PUT")
    {
        input.volatility_ = 0.6;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 17.94));
    }

    SECTION("ITM one year expiry low interest CALL")
    {
        input.riskFreeInterest_ = 0.01;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 10.33));
    }

    SECTION("OTM one year expiry low interest PUT")
    {
        input.riskFreeInterest_ = 0.01;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 4.38));
    }
}

TEST_CASE("Out of the money calls, in the money puts", "[otm]")
{
    OptionValues<value_type> input { 20.15, 35.20, 1, 0.25, 0.03 };

    SECTION("OTM one year expiry CALL")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 0.04));
    }

    SECTION("ITM one year expiry PUT")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 14.05));
    }

    SECTION("OTM half year expiry CALL")
    {
        input.timeToExpiry_ = 0.5;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 0.00));
    }

    SECTION("ITM half year expiry PUT")
    {
        input.timeToExpiry_ = 0.5;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 14.16));
    }

    SECTION("OTM quarter year expiry CALL")
    {
        input.timeToExpiry_ = 0.25;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 0.00));
    }

    SECTION("ITM quarter year expiry PUT")
    {
        input.timeToExpiry_ = 0.25;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 13.41));
    }

    SECTION("OTM one year expiry high volatility CALL")
    {
        input.volatility_ = 0.6;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 1.60));
    }

    SECTION("ITM one year expiry high volatility PUT")
    {
        input.volatility_ = 0.6;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 15.60));
    }

    SECTION("OTM one year expiry low interest CALL")
    {
        input.riskFreeInterest_ = 0.01;
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 0.03));
    }

    SECTION("ITM one year expiry low interest PUT")
    {
        input.riskFreeInterest_ = 0.01;
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 14.73));
    }
}

TEST_CASE("At the money CALL and PUT options", "[atm]")
{
    OptionValues<value_type> input { 40.50, 40.50, 1.5, 0.3, 0.01 };

    SECTION("ATM one and a half year expiry CALL")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 6.17));
    }

    SECTION("ATM one and a half year expiry PUT")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option(), 5.56));
    }

    SECTION("ATM one and a half year expiry high volatility CALL")
    {
        Option<CallExecutor> option(std::move(input));
        input.volatility_ = 0.7;
        REQUIRE(compareFloat(option(), 6.17));
    }

    SECTION("ATM one and a half year expiry high volatility PUT")
    {
        Option<PutExecutor> option(std::move(input));
        input.volatility_ = 0.7;
        REQUIRE(compareFloat(option(), 5.56));
    }

    SECTION("ATM one and a half year expiry high interest CALL")
    {
        Option<CallExecutor> option(std::move(input));
        input.riskFreeInterest_ = 0.2;
        REQUIRE(compareFloat(option(), 6.17));
    }

    SECTION("ATM one and a half year expiry high interest PUT")
    {
        Option<PutExecutor> option(std::move(input));
        input.riskFreeInterest_ = 0.2;
        REQUIRE(compareFloat(option(), 5.56));
    }
}
