#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

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

const auto NUM_BASE_FLAGS = baseOpts.size() / 2;
const auto NUM_ADDL_FLAGS = addOpts.size() / 2;
const auto NUM_ALL_FLAGS  = NUM_BASE_FLAGS + NUM_ADDL_FLAGS;

constexpr const auto IMPIED_VOL = 0.18;
constexpr const auto INTEREST   = 0.02;

} // bsm

#endif
