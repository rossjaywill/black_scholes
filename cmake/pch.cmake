target_precompile_headers(
    bsm PRIVATE
    <cmath>
    <cstdint>
    <cstdlib>
    <stdexcept>
)

target_precompile_headers(
    bsm PRIVATE
    <iostream>
    <functional>
    <unordered_map>
    <vector>
    <chrono>
    <iomanip>
    <sstream>
    <string>
    "include/constants.h"
)

