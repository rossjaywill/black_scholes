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
    "-v", "-r",
    "--volatility", "--rate-of-interest"
};

static ArgLookup paramLookup {
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

static const uint32_t NUM_BASE_FLAGS = baseOpts.size() / 2;
static const uint32_t NUM_ADDL_FLAGS = addOpts.size() / 2;
static const uint32_t NUM_ALL_FLAGS  = NUM_BASE_FLAGS + NUM_ADDL_FLAGS;

class ArgParser
{
public:
    ArgParser() = default;

    bool populateArgs(const Args &params);
    OptionValues<double> getOptionValues();
    OptionType getOptionType();
    size_t getNumberArgs() const { return arguments_.size(); }

private:
    // Parse input dates in format: 'YYYY-mm-dd'
    uint32_t parseDate(std::string_view date) const;
    bool validateFlags(const Args &flags);
    OptionMap arguments_;
};

} //bsm
