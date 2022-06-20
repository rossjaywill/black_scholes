#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "constants.h"

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

bool validateFlags(const Args &flags, OptionMap &options)
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
        return true;
    }
    else if (flags.size() == NUM_BASE_FLAGS) {
        if (!std::all_of(flags.begin(), flags.end(), withinBase)) {
            return false;
        }
        else {
            options[Flag::Interest] = std::to_string(INTEREST);
            options[Flag::Volatility] = std::to_string(IMPIED_VOL);
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

bool populateArgs(const Args &params, OptionMap &options)
{
    Args flags;
    for (const auto &param : params) {
        if (param.find_first_of("-") == 0) {
            flags.push_back(param);
        }
    }

    if (!validateFlags(flags, options)) {
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        const auto &in = params[i];
        if (std::find(in.cbegin(), in.cend(), '-') != in.end()) {
            if (paramLookup.contains(in)) {
                options[paramLookup[in]] = params[i + 1];
            }
        }
    }

    return true;
}

} //bsm
