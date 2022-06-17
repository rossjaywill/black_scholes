#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include "option.h"

#include <vector>

namespace bsm
{

class BlackScholes
{
public:
    BlackScholes() = default;
    auto valuesAtExpiry();

private:
    std::vector<Option<double>> options_;
};

} // bsm

#endif
