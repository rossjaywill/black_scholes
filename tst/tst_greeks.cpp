#include "constants.h"
#include "options.h"
#include "tst_helpers.h"

#include "catch2/catch.hpp"

using namespace bsm;

TEST_CASE("Greek calculations derived from ITM CALL and OTM PUTs", "[gitm]")
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

    // SECTION("ITM one year expiry CALL gamma")
    // {
    //     Option<CallExecutor> option(std::move(input));
    //     // TODO RJW: Should this be 0.018 - recalculate and verify
    //     REQUIRE(compareFloat(option.gamma(), 0.026, DP3));
    // }
    //
    // SECTION("OTM one year expiry PUT gamma")
    // {
    //     Option<PutExecutor> option(std::move(input));
    //     REQUIRE(compareFloat(option.gamma(), 0.704, DP3));
    // }
}

TEST_CASE("Greek calculations derived from OTM CALL and ITM PUTs", "[gitm]")
{
    OptionValues<value_type> input { 40.50, 55.75, 1.25, 0.10, 0.08 };

    SECTION("OTM one year expiry CALL delta")
    {
        Option<CallExecutor> option(std::move(input));
        REQUIRE(compareFloat(option.delta(), 0.018, DP3));
    }

    // SECTION("OTM one year expiry CALL gamma")
    // {
    //     Option<CallExecutor> option(std::move(input));
    //     REQUIRE(compareFloat(option.gamma(), 0.704, DP3));
    // }
}
