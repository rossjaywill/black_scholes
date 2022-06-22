#include "arg_parser.h"
#include "constants.h"
#include "tst_helpers.h"

#include "catch2/catch.hpp"

using namespace bsm;
using Catch::Matchers::Contains;

TEST_CASE("Input interface to bsm binary", "[input]")
{
    ArgParser parser;

    REQUIRE(parser.getNumberArgs() == 0);

    SECTION("Verify all values correct with date")
    {
        const auto dateOffset = 8;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(dateOffset), "-r", "0.02", "-v", "0.15" };
        parser.populateArgs(in);

        auto values = parser.getOptionValues();
        REQUIRE(values.underlyingPrice_  == 95.0);
        REQUIRE(values.strikePrice_      == 100.0);
        REQUIRE(values.timeToExpiry_     == dateOffset);
        REQUIRE(values.riskFreeInterest_ == 0.02);
        REQUIRE(values.volatility_       == 0.15);
    }

    SECTION("Verify default interest and volatiliity values with date")
    {
        const auto dateOffset = 14;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(dateOffset)};
        parser.populateArgs(in);

        auto values = parser.getOptionValues();
        REQUIRE(values.underlyingPrice_  == 95.0);
        REQUIRE(values.strikePrice_      == 100.0);
        REQUIRE(values.timeToExpiry_     == dateOffset);
        REQUIRE(values.riskFreeInterest_ == INTEREST);
        REQUIRE(values.volatility_       == IMPIED_VOL);
    }

    SECTION("Ensure already expired options are not handled")
    {
        const auto dateOffset = -1;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(dateOffset)};
        parser.populateArgs(in);
        REQUIRE_THROWS(parser.getOptionValues(), Contains("already expired"));
    }

    SECTION("Verify days to expiry cannot be greater than 10 years")
    {
        const auto dateOffset = 10 * DAY_TO_YEAR + 1;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(dateOffset)};
        parser.populateArgs(in);
        REQUIRE_THROWS(parser.getOptionValues(), Contains("expiry cannot be greater than"));
    }

    SECTION("Verify interest rate as ratio cannot be negative")
    {
        const auto dateOffset = 6;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(dateOffset), "-v", "0.18", "-r", "-0.3" };
        parser.populateArgs(in);
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Decimal percentage"), Contains("less than zero"));
    }

    SECTION("Verify interest rate as ratio cannot be greater than one")
    {
        const auto dateOffset = 11;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(dateOffset), "-v", "0.18", "-r", "1.05" };
        parser.populateArgs(in);
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Decimal percentage"), Contains("greater than one"));
    }

    SECTION("Verify volatility as ratio cannot be negative")
    {
        const auto dateOffset = 15;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(dateOffset), "-v", "-0.18", "-r", "0.3" };
        parser.populateArgs(in);
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Decimal percentage"), Contains("less than zero"));
    }

    SECTION("Verify volatility as ratio cannot be greater than one")
    {
        const auto dateOffset = 30;
        const Args in { "-o", "call", "-u", "95.00", "-s", "100", "-t", getDateOffset(dateOffset), "-v", "1.18", "-r", "0.99" };
        parser.populateArgs(in);
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Decimal percentage"), Contains("greater than one"));
    }

    SECTION("Verify underlying price cannot be negative")
    {
        const auto dateOffset = 15;
        const Args in { "-o", "call", "-u", "-1", "-s", "1.00", "-t", getDateOffset(dateOffset), "-v", "0.99", "-r", "0.3" };
        parser.populateArgs(in);
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Price"), Contains("less than zero"));
    }

    SECTION("Verify underlying price cannot be greater than the max price")
    {
        const auto dateOffset = 30;
        const Args in { "-o", "call", "-u", "100001.00", "-s", "100.00", "-t", getDateOffset(dateOffset), "-v", "0.01", "-r", "0.99" };
        parser.populateArgs(in);
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Price"), Contains("greater than 100000"));
    }

    SECTION("Verify strike price cannot be negative")
    {
        const auto dateOffset = 15;
        const Args in { "-o", "call", "-u", "100.00", "-s", "-999.99", "-t", getDateOffset(dateOffset), "-v", "0.99", "-r", "0.3" };
        parser.populateArgs(in);
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Price"), Contains("less than zero"));
    }

    SECTION("Verify strike price cannot be greater than the max price")
    {
        const auto dateOffset = 30;
        const Args in { "-o", "call", "-u", "95.00", "-s", "99999999.99", "-t", getDateOffset(dateOffset), "-v", "0.01", "-r", "0.99" };
        parser.populateArgs(in);
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Price"), Contains("greater than 100000"));
    }
}
