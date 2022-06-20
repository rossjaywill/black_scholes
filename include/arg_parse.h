#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "constants.h"
#include "option.h"

namespace bsm
{

enum class Flag : char {
    OptionType = 'o',
    Underlying = 'u',
    Strike     = 's',
    Expiry     = 't',
    Volatility = 'v',
    Interest   = 'r',
};

using Args         = std::vector<std::string>;
using ArgLookup    = std::unordered_map<std::string, Flag>;
using Parser       = std::function<void(std::string_view)>;
using ParserLookup = std::unordered_map<Flag, Parser>;
using OptionMap    = std::unordered_map<Flag, std::string>;
using ArgKeys      = std::vector<Flag>;

class ArgParser
{
public:
    ArgParser() = default;
    bool populateArgs(const Args &params)
    {
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

    OptionValues<double> getOptionValues() {
        const auto underlying = std::stod(arguments_[Flag::Underlying]);
        const auto strike     = std::stod(arguments_[Flag::Strike]);
        const auto expiry     = parseDate(arguments_[Flag::Expiry]);
        const auto volatility = std::stod(arguments_[Flag::Volatility]);
        const auto interest   = std::stod(arguments_[Flag::Interest]);

        return OptionValues<double> { underlying, strike, expiry, volatility, interest };
    }

    OptionType getOptionType() {
        if (arguments_[Flag::OptionType] == "call") {
            return OptionType::Call;
        }
        return OptionType::Put;
    }

    size_t getNumberArgs() { return arguments_.size(); }

private:
    // Parse input dates in format: 'YYYY-mm-dd'
    uint32_t parseDate(std::string_view date) {
        using namespace std::chrono;
        // TODO RJW: Reimplement when libc++std updated
        // std::chrono::from_stream(std::stringstream(std::string(date)), "%Y-%m-%d", ymd);

        auto setToMidnight = [](std::tm &time) { time.tm_hour = 0; time.tm_min = 0; time.tm_sec = 0; };
        auto getDaysDelta  = [](double diff) { return static_cast<uint32_t>((((diff / 60) / 60) / 24)); };

        const std::time_t now = std::time(nullptr);
        std::tm inTime;
        std::tm* nowLocal = std::localtime(&now);

        std::stringstream ss(date.data());
        ss >> std::get_time(&inTime, "%Y-%m-%d");
        setToMidnight(inTime);
        setToMidnight(*nowLocal);

        const std::time_t expiry = std::mktime(&inTime);
        const std::time_t current = std::mktime(nowLocal);

        if (current > expiry) {
            throw std::runtime_error("Option has already expired, please enter a valid date");
        }

        auto diff = std::difftime(expiry, current);
        return getDaysDelta(diff);
    }

    bool validateFlags(const Args &flags)
    {
        Args fullOpts = baseOpts;
        for (const auto &opt : addOpts) { fullOpts.push_back(opt); }

        const auto withinBase = [&](std::string_view arg) { return std::find(baseOpts.begin(), baseOpts.end(), arg) != baseOpts.end(); };
        const auto withinAll  = [&](std::string_view arg) { return std::find(fullOpts.begin(), fullOpts.end(), arg) != fullOpts.end(); };

        if (std::any_of(flags.begin(), flags.end(), [](std::string_view arg) {
                    return (arg == "-h" || arg == "--help");
                })) {
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

    const Args baseOpts {
        "-o", "-u", "-s", "-t",
        "--option-type", "--underlying-price", "--strike-price", "--time-to-expiry"
    };
    const Args addOpts {
        "-v", "-r",
        "--volatility", "--rate-of-interest"
    };

    ArgLookup paramLookup {
        { "-o",                 Flag::OptionType },
        { "--option-type",      Flag::OptionType },
        { "-u",                 Flag::Underlying },
        { "--underlying-price", Flag::Underlying },
        { "-s",                 Flag::Strike },
        { "--strike-price",     Flag::Strike },
        { "-t",                 Flag::Expiry },
        { "--time-to-expiry",   Flag::Expiry },
        { "-v",                 Flag::Volatility },
        { "--volatility",       Flag::Volatility },
        { "-r",                 Flag::Interest },
        { "--rate-of-interest", Flag::Interest },
    };

    const uint32_t NUM_BASE_FLAGS = baseOpts.size() / 2;
    const uint32_t NUM_ADDL_FLAGS = addOpts.size() / 2;
    const uint32_t NUM_ALL_FLAGS  = NUM_BASE_FLAGS + NUM_ADDL_FLAGS;

    OptionMap arguments_;
};

} //bsm
