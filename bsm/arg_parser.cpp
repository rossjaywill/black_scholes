#include "arg_parser.h"
#include "constants.h"
#include "helpers.h"

namespace bsm
{

auto ArgParser::populateArgs(const Args &params) -> bool {
    Args flags;
    for (const auto &param : params) {
        if (param.find_first_of("-") == 0) {
            flags.push_back(param);
        }
    }

    if (!validateFlags(flags)) {
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        const auto &in = params[i];
        if (std::find(in.cbegin(), in.cend(), '-') != in.end()) {
            if (paramLookup.contains(in)) {
                arguments_[paramLookup[in]] = params[i + 1];
            }
        }
    }

    return true;
}

auto ArgParser::getOptionValues() -> OptionValues<ArgParser::value_type> {
    const auto underlying = std::stod(arguments_[Flag::Underlying]);
    const auto strike     = std::stod(arguments_[Flag::Strike]);
    const auto expiry     = parseDate(arguments_[Flag::Expiry]);
    const auto volatility = std::stod(arguments_[Flag::Volatility]);
    const auto interest   = std::stod(arguments_[Flag::Interest]);
    const auto yield      = std::stod(arguments_[Flag::Dividend]);

    return OptionValues<value_type> { underlying, strike, expiry, volatility, interest, yield };
}

auto ArgParser::getOptionType() -> OptionType {
    auto &input = arguments_[Flag::OptionType];
    std::transform(input.begin(), input.end(), input.begin(), [](auto c) {
            return std::tolower(c);
    });
    return (input.starts_with("c")) ? OptionType::Call : OptionType::Put;
}

auto ArgParser::validateFlags(const Args &flags) -> bool {
    Args fullOpts = baseOpts;
    for (const auto &opt : addOpts) {
        fullOpts.push_back(opt);
    }

    const auto withinAll  = [&](std::string_view arg) {
        return std::find(fullOpts.begin(), fullOpts.end(), arg) != fullOpts.end();
    };
    const auto withinHelp = [&](std::string_view arg) {
        return (arg == "-h" || arg == "--help");
    };

    const auto hasInterest = [&](std::string_view arg) {
        return (arg == "-r" || arg == "--rate-of-interest");
    };
    const auto hasVolatility = [&](std::string_view arg) {
        return (arg == "-v" || arg == "--volatility");
    };
    const auto hasDividend = [&](std::string_view arg) {
        return (arg == "-d" || arg == "--dividend");
    };

    if (std::any_of(flags.begin(), flags.end(), withinHelp)) {
        return false;
    }

    if (flags.size() >= NUM_BASE_FLAGS && flags.size() <= NUM_ALL_FLAGS){
        if (!std::any_of(flags.begin(), flags.end(), withinAll)) {
            return false;
        }

        if (!std::any_of(flags.begin(), flags.end(), hasInterest)) {
            arguments_[Flag::Interest] = std::to_string(INTEREST);
        }
        if (!std::any_of(flags.begin(), flags.end(), hasVolatility)) {
            arguments_[Flag::Volatility] = std::to_string(IMPLIED_VOL);
        }
        if (!std::any_of(flags.begin(), flags.end(), hasDividend)) {
            arguments_[Flag::Dividend]   = std::to_string(YIELD);
        }
    }
    else {
        return false;
    }

    return true;
}

} // bsm
