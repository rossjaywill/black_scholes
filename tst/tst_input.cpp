#include "arg_parse.h"
#include "constants.h"

#include "catch2/catch.hpp"

using namespace bsm;

TEST_CASE("Input interface to bsm binary", "[input]")
{
    ArgParser parser;
    OptionMap arguments;

    REQUIRE(arguments.size() == 0);

    SECTION("Verify all values correct input")
    {
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", "2022-06-16", "-r", "0.02", "-v", "0.15" };
        parser.populateArgs(in, arguments);

        REQUIRE(arguments[Flag::OptionType] == "call");
        REQUIRE(arguments[Flag::Underlying] == "95");
        REQUIRE(arguments[Flag::Strike]     == "100");
        REQUIRE(arguments[Flag::Expiry]     == "2022-06-16");
        REQUIRE(arguments[Flag::Interest]   == "0.02");
        REQUIRE(arguments[Flag::Volatility] == "0.15");
    }

    SECTION("Verify default interest and volatiliity values added")
    {
        const Args in { "-o", "call", "-u", "95", "-s", "100", "-t", "2022-06-16" };
        parser.populateArgs(in, arguments);
        REQUIRE(arguments[Flag::OptionType] == "call");
        REQUIRE(arguments[Flag::Underlying] == "95");
        REQUIRE(arguments[Flag::Strike]     == "100");
        REQUIRE(arguments[Flag::Expiry]     == "2022-06-16");
        REQUIRE(arguments[Flag::Interest]   == std::to_string(INTEREST));
        REQUIRE(arguments[Flag::Volatility] == std::to_string(IMPIED_VOL));
    }
}
