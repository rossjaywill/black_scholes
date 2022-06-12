#include <iostream>
#include <fmt/core.h>

#include "constants.h"

int main()
{
    fmt::print("Hello from fmt with e: {} !\n", black_scholes::EULER_NUM);
    return 0;
}
