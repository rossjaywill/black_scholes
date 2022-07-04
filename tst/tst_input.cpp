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
        const value_type daysOffset = 30;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(daysOffset),
            "-r", "0.02", "-v", "0.15", "-d", "0.10"
        };
        REQUIRE(parser.populateArgs(in));

        auto values = parser.getOptionValues();
        REQUIRE(values.underlyingPrice_  == 95.0);
        REQUIRE(values.strikePrice_      == 100.0);
        REQUIRE(compareFloat(values.timeToExpiry_, daysOffset / DAY_TO_YEAR, DP3));
        REQUIRE(compareFloat(values.riskFreeInterest_, 0.02));
        REQUIRE(compareFloat(values.volatility_, 0.15));
        REQUIRE(compareFloat(values.dividendYield_, 0.10));
    }

    SECTION("Verify default interest and volatiliity values with date")
    {
        const value_type daysOffset = 14;
        const Args in { "-o", "call", "-u", "95.00", "-s", "100.00", "-t", getDateOffset(daysOffset) };
        REQUIRE(parser.populateArgs(in));

        auto values = parser.getOptionValues();
        REQUIRE(values.underlyingPrice_  == 95.00);
        REQUIRE(values.strikePrice_      == 100.00);
        REQUIRE(compareFloat(values.timeToExpiry_, daysOffset / DAY_TO_YEAR, DP3));
        REQUIRE(compareFloat(values.riskFreeInterest_, INTEREST));
        REQUIRE(compareFloat(values.volatility_,       IMPLIED_VOL));
        REQUIRE(compareFloat(values.dividendYield_,    YIELD));
    }

    SECTION("Ensure already expired options are not handled")
    {
        const value_type daysOffset = -1;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(daysOffset) };
        REQUIRE(parser.populateArgs(in));
        REQUIRE_THROWS(parser.getOptionValues(), Contains("already expired"));
    }

    SECTION("Verify days to expiry cannot be greater than 10 years")
    {
        const value_type daysOffset = 11 * DAY_TO_YEAR;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(daysOffset) };
        REQUIRE(parser.populateArgs(in));
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Time to expiry cannot be greater than"));
    }

    SECTION("Verify interest rate as ratio cannot be negative")
    {
        const value_type daysOffset = 6;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(daysOffset), "-v", "0.18", "-r", "-0.3" };
        REQUIRE(parser.populateArgs(in));
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Decimal percentage"), Contains("less than zero"));
    }

    SECTION("Verify interest rate as ratio cannot be greater than one")
    {
        const value_type daysOffset = 11;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(daysOffset), "-v", "0.18", "-r", "1.05" };
        REQUIRE(parser.populateArgs(in));
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Decimal percentage"), Contains("greater than one"));
    }

    SECTION("Verify volatility as ratio cannot be negative")
    {
        const value_type daysOffset = 15;
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", getDateOffset(daysOffset), "-v", "-0.18", "-r", "0.3" };
        REQUIRE(parser.populateArgs(in));
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Decimal percentage"), Contains("less than zero"));
    }

    SECTION("Verify volatility as ratio cannot be greater than one")
    {
        const value_type daysOffset = 30;
        const Args in { "-o", "call", "-u", "95.00", "-s", "100", "-t", getDateOffset(daysOffset), "-v", "1.18", "-r", "0.99" };
        REQUIRE(parser.populateArgs(in));
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Decimal percentage"), Contains("greater than one"));
    }

    SECTION("Verify underlying price cannot be negative")
    {
        const value_type daysOffset = 15;
        const Args in { "-o", "call", "-u", "-1", "-s", "1.00", "-t", getDateOffset(daysOffset), "-v", "0.99", "-r", "0.3" };
        REQUIRE(parser.populateArgs(in));
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Price"), Contains("less than zero"));
    }

    SECTION("Verify underlying price cannot be greater than the max price")
    {
        const value_type daysOffset = 30;
        const Args in { "-o", "call", "-u", "100001.00", "-s", "100.00", "-t", getDateOffset(daysOffset), "-v", "0.01", "-r", "0.99" };
        REQUIRE(parser.populateArgs(in));
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Price"), Contains("greater than 100000"));
    }

    SECTION("Verify strike price cannot be negative")
    {
        const value_type daysOffset = 15;
        const Args in { "-o", "call", "-u", "100.00", "-s", "-999.99", "-t", getDateOffset(daysOffset), "-v", "0.99", "-r", "0.3" };
        REQUIRE(parser.populateArgs(in));
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Price"), Contains("less than zero"));
    }

    SECTION("Verify strike price cannot be greater than the max price")
    {
        const value_type daysOffset = 30;
        const Args in { "-o", "call", "-u", "95.00", "-s", "99999999.99", "-t", getDateOffset(daysOffset), "-v", "0.01", "-r", "0.99" };
        REQUIRE(parser.populateArgs(in));
        REQUIRE_THROWS(parser.getOptionValues(), Contains("Price"), Contains("greater than 100000"));
    }
}
