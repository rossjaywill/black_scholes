#include <array>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <set>
#include <fmt/core.h>

#include "arg_parse.h"
#include "constants.h"
#include "option.h"

using namespace bsm;

void displayHelp()
{
    fmt::print("Direct run of BSM command require five arguments:\n"
                "\t-o | --option-type          : Type of option ('call' or 'put')\n"
                "\t-u | --underling-price      : Price of underlying asset\n"
                "\t-s | --strike-price         : Strike price of the options contract\n"
                "\t-t | --time-to-expiry       : Time to next (front) expiry date of the option contract in days\n\n"
                "\t-v | --volatility           : Implied volatility of underlying asset (optional)\n"
                "\t-r | --rate-of-interest     : Risk Free interest rate (optional)\n"
                "\t(-h | --help                : Display this help/usage message)\n"
                "Optionally, if implied-volatility and interest-rate are omitted, the program will use "
                "default assumptions for these values.\n");
}

[[noreturn]] void helpAndExit(int code)
{
    displayHelp();
    exit(code);
}

int main(int argc, char **argv)
{
    OptionMap arguments;
    try {
        fmt::print("Running black scholes merton\n");

        const Args params { argv + 1, argv + argc };
        if (!populateArgs(params, arguments)) {
            return 1;
        }

        const auto underlying = std::stod(arguments[Flag::Underlying]);
        const auto strike = std::stod(arguments[Flag::Strike]);
        const auto expiry = static_cast<uint32_t>(std::stoi(arguments[Flag::Expiry]));
        const auto interest = std::stod(arguments[Flag::Interest]);
        const auto volatility = std::stod(arguments[Flag::Volatility]);

        if (arguments[Flag::OptionType] == "call") {
            Option<CallOption, double> call(underlying, strike, expiry, interest, volatility);
            auto value = call();
            fmt::print("Call Option Value: {}\n", value);
        }
        else {
            Option<PutOption, double> put(underlying, strike, expiry, interest, volatility);
            auto value = put();
            fmt::print("Put Option Value: {}\n", value);
        }
    }
    catch (const std::exception &e) {
        fmt::print("Exeception caught during bsm run:\n{}", e.what());
        return 1;
    }
    return 0;
}
