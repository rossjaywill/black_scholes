#include <array>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <set>
#include <fmt/core.h>

#include "black_scholes.h"
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

void validateFlags(const Args &flags, OptionMap &options)
{
    Args fullOpts = baseOpts;
    for (const auto &opt : addOpts) {
        fullOpts.push_back(opt);
    }

    const auto withinBase = [&](std::string_view arg) { return std::find(baseOpts.begin(), baseOpts.end(), arg) != baseOpts.end(); };
    const auto withinAll  = [&](std::string_view arg) { return std::find(fullOpts.begin(), fullOpts.end(), arg) != fullOpts.end(); };

    if (std::any_of(flags.begin(), flags.end(), [](std::string_view arg) {
                return (arg == "-h" || arg == "--help");
            })) {
        helpAndExit(0);
    }
    else if (flags.size() == NUM_BASE_FLAGS) {
        if (!std::all_of(flags.begin(), flags.end(), withinBase)) {
            helpAndExit(1);
        }
        else {
            options[Flag::Interest] = std::to_string(INTEREST);
            options[Flag::Volatility] = std::to_string(IMPIED_VOL);
        }
    }
    else if (flags.size() == NUM_ALL_FLAGS) {
        if (!std::all_of(flags.begin(), flags.end(), withinAll)) {
            helpAndExit(1);
        }
    }
    else {
        helpAndExit(1);
    }
}

void populateArgs(const Args &params, OptionMap &options)
{
    Args flags;
    for (const auto &param : params) {
        if (param.find_first_of("-") == 0) {
            flags.push_back(param);
        }
    }

    validateFlags(flags, options);

    for (size_t i = 0; i < params.size(); ++i) {
        const auto &in = params[i];
        if (std::find(in.cbegin(), in.cend(), '-') != in.end()) {
            if (paramLookup.contains(in)) {
                options[paramLookup[in]] = params[i + 1];
            }
        }
    }
}

int main(int argc, char **argv)
{
    OptionMap arguments;
    try {
        fmt::print("Running black scholes merton: {}\n", EULER_NUM);

        const Args params { argv + 1, argv + argc };
        populateArgs(params, arguments);

        // const auto optionType = (arguments[Flag::OptionType] == "call") ? CallOption{} : PutOption{};
        using OptionType = CallOption;
        if (arguments[Flag::OptionType] == "call") {
            using OptionType = CallOption;
        }
        else {
            using OptionType = PutOption;
        }

        const auto underlying = std::stod(arguments[Flag::Underlying]);
        const auto strike = std::stod(arguments[Flag::Strike]);
        const auto expiry = static_cast<uint32_t>(std::stoi(arguments[Flag::Expiry]));
        const auto interest = std::stod(arguments[Flag::Interest]);
        const auto volatility = std::stod(arguments[Flag::Volatility]);

        Option<OptionType, double> option(underlying, strike, expiry, interest, volatility);
    }
    catch (const std::exception &e) {
        fmt::print("Exeception caught during bsm run:\n{}", e.what());
    }
    return 0;
}
