#include "arg_parser.h"
#include "constants.h"

#include "catch2/catch.hpp"

using namespace bsm;
using Catch::Matchers::Contains;

TEST_CASE("Input interface to bsm binary", "[input]")
{
    ArgParser parser;

    REQUIRE(parser.getNumberArgs() == 0);

    SECTION("Verify all values correct with date")
    {
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", "2222-06-16", "-r", "0.02", "-v", "0.15" };
        parser.populateArgs(in);

        auto values = parser.getOptionValues();
        REQUIRE(values.underlyingPrice_  == 95.0);
        REQUIRE(values.strikePrice_      == 100.0);
        // REQUIRE(values                      == "2022-06-16");
        REQUIRE(values.riskFreeInterest_ == 0.02);
        REQUIRE(values.volatility_       == 0.15);
    }

    SECTION("Verify default interest and volatiliity values with date")
    {
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", "2222-06-16" };
        parser.populateArgs(in);

        auto values = parser.getOptionValues();
        REQUIRE(values.underlyingPrice_  == 95.0);
        REQUIRE(values.strikePrice_      == 100.0);
        // REQUIRE(values                      == "2022-06-16");
        REQUIRE(values.riskFreeInterest_ == INTEREST);
        REQUIRE(values.volatility_       == IMPIED_VOL);
    }

    SECTION("Ensure already expired options are not handled")
    {
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", "2020-06-16" };
        parser.populateArgs(in);
        REQUIRE_THROWS(parser.getOptionValues(), Contains("already expired"));
    }
}
