#include <algorithm>
#include <iostream>
#include <set>
#include <string_view>
#include <fmt/core.h>

#include "black_scholes.h"
#include "constants.h"
#include "option.h"

using namespace black_scholes;

void displayHelp()
{
    fmt::print("Direct run of BSM command require five arguments:\n"
                "\t-o | --option-type          : Type of option ('call' or 'put')\n"
                "\t-u | --underling-price      : Price of underlying asset\n"
                "\t-s | --strike-price         : Strike price of the options contract\n"
                "\t-v | --implied-volatility   : Implied volatility of underlying asset\n"
                "\t-r | --interest-rate        : Risk Free interest rate\n"
                "\t-t | --time-to-expiry       : Time to next (front) expiry date of the option contract in days\n\n"
                "\t(-h | --help                 : Display this help/usage message)\n"
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
    const std::set<std::string_view> baseOpts {
        "-o", "-u", "-s", "-t",
        "--option-type", "--underlying-price", "--strike-price", "--time-to-expiry"
    };
    const std::vector<std::string_view> addOpts = {
        "-v", "-r", "--interest-rate", "--time-to-expiry"
    };
    std::set<std::string_view> fullOpts = baseOpts;
    for (const auto &opt : addOpts) {
        fullOpts.insert(opt);
    }

    const std::set<std::string_view> params { argv + 1, argv + argc };

    auto within = [&fullOpts](std::string_view arg) { return fullOpts.find(arg) != fullOpts.end(); };

    if (std::any_of(params.begin(), params.end(), [](std::string_view arg) {
                return (arg == "-h" || arg == "--help");
            })) {
        helpAndExit(0);
    }
    else if (params.size() == 4 &&
            std::all_of(params.begin(), params.end(), within)) {
        fmt::print("All 3 found");
    }
    else if (params.size() == 6 &&
            std::all_of(params.begin(), params.end(), within)) {
        fmt::print("All 5 found");
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
