#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <functional>
#include <unordered_map>
#include <vector>

#include "constants.h"
#include "options.h"

namespace bsm
{

enum class Flag : char {
    OptionType = 'o',
    Underlying = 'u',
    Strike     = 's',
    Expiry     = 't',
    Volatility = 'v',
    Interest   = 'r',
    Dividend   = 'd',
};

using Args         = std::vector<std::string>;
using ArgLookup    = std::unordered_map<std::string, Flag>;
using Parser       = std::function<void(std::string_view)>;
using ParserLookup = std::unordered_map<Flag, Parser>;
using OptionMap    = std::unordered_map<Flag, std::string>;
using ArgKeys      = std::vector<Flag>;

static const Args baseOpts {
    "-o", "-u", "-s", "-t",
    "--option-type", "--underlying-price", "--strike-price", "--time-to-expiry"
};

static const Args addOpts {
    "-v", "-r", "-d",
    "--volatility", "--rate-of-interest", "--dividend"
};

static ArgLookup paramLookup{
    {"-o", Flag::OptionType}, {"--option-type",      Flag::OptionType},
    {"-u", Flag::Underlying}, {"--underlying-price", Flag::Underlying},
    {"-s", Flag::Strike},     {"--strike-price",     Flag::Strike},
    {"-t", Flag::Expiry},     {"--time-to-expiry",   Flag::Expiry},
    {"-v", Flag::Volatility}, {"--volatility",       Flag::Volatility},
    {"-r", Flag::Interest},   {"--rate-of-interest", Flag::Interest},
    {"-d", Flag::Dividend},   {"--dividend",         Flag::Dividend},
};

static const uint32_t NUM_BASE_FLAGS = baseOpts.size() / 2;
static const uint32_t NUM_ADDL_FLAGS = addOpts.size() / 2;
static const uint32_t NUM_ALL_FLAGS  = NUM_BASE_FLAGS + NUM_ADDL_FLAGS;

class ArgParser
{
public:
    using value_type = double;
    ArgParser() = default;

    auto populateArgs(const Args &params) -> bool;
    auto getOptionValues() -> OptionValues<value_type>;
    auto getOptionType() -> OptionType;
    auto getNumberArgs() const -> size_t { return arguments_.size(); }

private:
    auto validateFlags(const Args &flags) -> bool;

    OptionMap arguments_;
};

} //bsm

#endif
