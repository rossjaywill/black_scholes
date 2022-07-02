#include "constants.h"
#include "options.h"
#include "tst_helpers.h"

#include "catch2/catch.hpp"

using namespace bsm;

TEST_CASE("Greek calculations derived from ITM CALL and OTM PUTs", "[itm_greeks]")
{
    OptionValues<value_type> input { 100.00, 95.00, 1, 0.18, 0.05 };

    SECTION("ITM one year expiry CALL delta")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.delta(), 0.743, DP3));
    }

    SECTION("OTM one year expiry PUT delta")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.delta(), -0.257, DP3));
    }

    SECTION("ITM one year expiry CALL gamma")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.gamma(), 0.019, DP3));
    }

    SECTION("OTM one year expiry PUT gamma")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.gamma(), 0.019, DP3));
    }

    SECTION("ITM one year expiry CALL theta")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.theta(), -5.982, DP3));
    }

    SECTION("OTM one year expiry PUT theta")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.theta(), -1.464, DP3));
    }

    SECTION("ITM one year expiry CALL vega")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.vega(), 32.240, DP3));
    }

    SECTION("OTM one year expiry PUT vega")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.vega(), 32.240, DP3));
    }
    SECTION("ITM one year expiry CALL rho")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.rho(), 61.612, DP3));
    }

    SECTION("OTM one year expiry PUT rho")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.rho(), -28.755, DP3));
    }
}

TEST_CASE("Greek calculations derived from OTM CALL and ITM PUTs", "[otm_greeks]")
{
    OptionValues<value_type> input { 40.00, 56.00, 1, 0.10, 0.08 };

    SECTION("OTM one year expiry CALL delta")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.delta(), 0.006, DP3));
    }

    SECTION("ITM one year expiry PUT delta")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.delta(), -0.994, DP3));
    }

    SECTION("OTM one year expiry CALL gamma")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.gamma(), 0.004, DP3));
    }

    SECTION("ITM one year expiry PUT gamma")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.gamma(), 0.004, DP3));
    }

    SECTION("OTM one year expiry CALL theta")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.theta(), -0.052, DP3));
    }

    SECTION("ITM one year expiry PUT theta")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.theta(), 4.083, DP3));
    }

    SECTION("OTM one year expiry CALL vega")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.vega(), 0.676, DP3));
    }

    SECTION("ITM one year expiry PUT vega")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.vega(), 0.676, DP3));
    }

    SECTION("OTM one year expiry CALL rho")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.rho(), 0.231, DP3));
    }

    SECTION("ITM one year expiry PUT rho")
    {
        Option<PutExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.rho(), -51.464, DP3));
    }
}
