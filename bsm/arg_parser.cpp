#include <chrono>
#include <iomanip>
#include <sstream>

#include "arg_parser.h"
#include "constants.h"

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

    return OptionValues<value_type> { underlying, strike, expiry, volatility, interest };
}

auto ArgParser::getOptionType() -> OptionType {
    return (arguments_[Flag::OptionType] == "call") ?
        OptionType::Call : OptionType::Put;
}

auto ArgParser::parseDate(std::string_view date) const -> ArgParser::value_type {
    // TODO RJW: Reimplement when libc++std 20 updated with 'from_stream'

    constexpr auto setToMidnight = [](std::tm &time) {
        time.tm_hour = 0;
        time.tm_min  = 0;
        time.tm_sec  = 0;
    };
    constexpr auto getDaysDelta  = [](value_type diff) {
        return static_cast<value_type>((((diff / SEC_TO_MIN) / MIN_TO_HOUR) / HOUR_TO_DAY));
    };

    const std::time_t now = std::time(nullptr);
    std::tm inTime {};
    std::tm nowLocal = *std::localtime(&now);

    std::stringstream ss(date.data());
    ss >> std::get_time(&inTime, DATE_FMT);
    setToMidnight(inTime);
    setToMidnight(nowLocal);

    const std::time_t expiry  = std::mktime(&inTime);
    const std::time_t current = std::mktime(&nowLocal);

    if (current > expiry) {
        throw std::runtime_error("Option has already expired, please enter a valid date");
    }

    auto diff = std::difftime(expiry, current);
    return getDaysDelta(diff) / DAY_TO_YEAR;
}

auto ArgParser::validateFlags(const Args &flags) -> bool {
    Args fullOpts = baseOpts;
    for (const auto &opt : addOpts) {
        fullOpts.push_back(opt);
    }

    const auto withinBase = [&](std::string_view arg) {
        return std::find(baseOpts.begin(), baseOpts.end(), arg) != baseOpts.end();
    };
    const auto withinAll  = [&](std::string_view arg) {
        return std::find(fullOpts.begin(), fullOpts.end(), arg) != fullOpts.end();
    };
    const auto withinHelp = [&](std::string_view arg) {
        return (arg == "-h" || arg == "--help");
    };

    if (std::any_of(flags.begin(), flags.end(), withinHelp)) {
        return true;
    }
    else if (flags.size() == NUM_BASE_FLAGS) {
        if (!std::all_of(flags.begin(), flags.end(), withinBase)) {
            return false;
        }
        else {
            arguments_[Flag::Interest] = std::to_string(INTEREST);
            arguments_[Flag::Volatility] = std::to_string(IMPIED_VOL);
        }
    }
    else if (flags.size() == NUM_ALL_FLAGS) {
        if (!std::all_of(flags.begin(), flags.end(), withinAll)) {
            return false;
        }
    }
    else {
        return false;
    }

    return true;
}

} // bsm
