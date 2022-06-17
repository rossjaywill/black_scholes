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

using OptionMap    = std::unordered_map<char, std::string>;
using ArgKeys      = std::vector<char>;

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

void validateFlags(const bsm::Args &flags, OptionMap &options)
{
    bsm::Args fullOpts;
    for (const auto &opt : bsm::addOpts) {
        fullOpts.push_back(opt);
    }

    const auto withinBase = [&](std::string_view arg) { return std::find(baseOpts.begin(), baseOpts.end(), arg) != baseOpts.end(); };
    const auto withinAll  = [&](std::string_view arg) { return std::find(fullOpts.begin(), fullOpts.end(), arg) != fullOpts.end(); };

    if (std::any_of(flags.begin(), flags.end(), [](std::string_view arg) {
                return (arg == "-h" || arg == "--help");
            })) {
        helpAndExit(0);
    }
    else if (flags.size() == bsm::NUM_BASE_ARGS) {
        if (!std::all_of(flags.begin(), flags.end(), withinBase)) {
            helpAndExit(1);
        }
        else {
            options['r'] = std::to_string(bsm::INTEREST);
            options['v'] = std::to_string(bsm::IMPIED_VOL);
        }
    }
    else if (flags.size() == bsm::NUM_ALL_ARGS) {
        if (!std::all_of(flags.begin(), flags.end(), withinAll)) {
            helpAndExit(1);
        }
    }
    else {
        helpAndExit(1);
    }
}

void populateArgs(const bsm::Args &params, OptionMap &options)
{
    bsm::Args flags;
    for (const auto &param : params) {
        if (param.find_first_of("-") == 0) {
            flags.push_back(param);
        }
    }

    fmt::print("num args: {}\n", params.size());

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
    fmt::print("Running black scholes merton algorithm: {}\n", bsm::EULER_NUM);
    OptionMap arguments;
    const bsm::Args params { argv + 1, argv + argc };

    populateArgs(params, arguments);

    for (const auto &arg : arguments) {
        fmt::print("arg key: {}, value: {}\n", arg.first, arg.second);
    }

    const auto optionType = (arguments['o'] == "call") ? CallPutFlag::Call : CallPutFlag::Put;
    const auto underlying = std::stod(arguments['u']);
    const auto strike = std::stod(arguments['s']);
    const auto expiry = static_cast<uint32_t>(std::stoi(arguments['t']));
    const auto interest = std::stod(arguments['r']);
    const auto volatility = std::stod(arguments['v']);

    Option<> option(optionType, underlying, strike, expiry, interest, volatility);
    // const auto optType = args['o'] == "call" ? CallPutFlag::Call : CallPutFlag::Put;
    // bsm::BlackScholes<>();
    return 0;
}
