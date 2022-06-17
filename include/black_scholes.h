#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include "option.h"

#include <vector>

namespace bsm
{

template<typename CallPut = CallOption>
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
