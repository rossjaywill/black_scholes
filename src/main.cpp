#include <algorithm>
#include <iostream>
#include <string_view>
#include <vector>
#include <fmt/core.h>

#include "black_scholes.h"
#include "constants.h"
#include "option.h"

using namespace black_scholes;

void displayHelp()
{
    fmt::print("Direct run of BSM command require five arguments:\n"
                "-o | --option-type          : Type of option ('call' or 'put')\n"
                "-u | --underling-price      : Price of underlying asset\n"
                "-s | --strike-price         : Strike price of the options contract\n"
                "-v | --implied-volatility   : Implied volatility of underlying asset\n"
                "-r | --interest-rate        : Risk Free interest rate\n"
                "-t | --time-to-expiry       : Time to next (front) expiry date of the option contract in days\n\n"
                "(-h | --help                 : Display this help/usage message)\n"
                "Optionally, if implied-volatility and interest-rate are omitted, the program will use "
                "default assumptions for these values.\n");
}

[[noreturn]] void helpAndExit(int code)
{
    displayHelp();
    exit(code);
}

void parseArgs(int argc, char **argv)
{
    const std::vector<std::string_view> params { argv + 1, argv + argc };

    if (params.size() == 1) {
        if (params[0] == "-h" || params[0] == "--help") {
            helpAndExit(0);
        }
        helpAndExit(1);
    }
    else if (params.size() == 3) {
        // run with assumptions
        BlackScholes<CallPutFlag> bsm;
    }
    else if (params.size() == 5) {
        // run without assumptions
    }
    else {
        helpAndExit(1);
    }
}

int main(int argc, char **argv)
{
    fmt::print("Running black scholes merton algorithm: {}\n", black_scholes::EULER_NUM);
    parseArgs(argc, argv);
    return 0;
}
