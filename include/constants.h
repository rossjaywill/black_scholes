#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace bsm
{
using Args = std::vector<std::string>;
using ArgLookup = std::unordered_map<std::string, char>;
using Parser       = std::function<void(std::string_view)>;
using ParserLookup = std::unordered_map<char, Parser>;

const Args baseOpts {
    "-o", "-u", "-s", "-t",
    "--option-type", "--underlying-price", "--strike-price", "--time-to-expiry"
};
const Args addOpts {
    "-v", "-r",
    "--volatility", "--rate-of-interest"
};

ArgLookup paramLookup {
    { "-o",                 'o' },
    { "--option-type",      'o' },
    { "-u",                 'u' },
    { "--underlying-price", 'u' },
    { "-s",                 's' },
    { "--strike-price",     's' },
    { "-t",                 't' },
    { "--time-to-expiry",   't' },
    { "-v",                 'v' },
    { "--volatility",       'v' },
    { "-r",                 'r' },
    { "--rate-of-interest", 'r' },
};

const auto parseOptionType = [](std::string_view input) {  };
const auto parsePrice      = [](std::string_view input) {  };
const auto parseDecimal    = [](std::string_view input) {  };
const auto parseSymbol     = [](std::string_view input) {  };
const auto parseDate       = [](std::string_view input) {  };

ParserLookup parsers {
    { 'o', parseOptionType },
    { 'u', parseSymbol },
    { 's', parsePrice },
    { 't', parseDate },
    { 'r', parseDecimal },
    { 'v', parseDecimal },
};

const auto NUM_BASE_ARGS = baseOpts.size() / 2;
const auto NUM_ADDL_ARGS = addOpts.size() / 2;
const auto NUM_ALL_ARGS  = NUM_BASE_ARGS + NUM_ADDL_ARGS;

constexpr const auto EULER_NUM  = 2.71828182845904523536;
constexpr const auto IMPIED_VOL = 0.18;
constexpr const auto INTEREST   = 0.02;

} // bsm

#endif
