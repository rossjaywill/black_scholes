#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include "option.h"

#include <vector>

namespace black_scholes
{

template <typename OptionType>
class BlackScholes
{
public:
    BlackScholes() = default;
    auto valuesAtExpiry();

private:
    std::vector<Option<double>> options_;
};

} // black_scholes

#endif
