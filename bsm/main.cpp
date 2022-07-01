#include <cstdlib>
#include <fmt/core.h>

#include "arg_parser.h"
#include "constants.h"
#include "options.h"

using namespace bsm;

void displayHelp() {
    fmt::print("Direct run of BSM command requires at least 4 of the required arguments:\n"
                "\t-o | --option-type          : Type of option ('call' or 'put') [required]\n"
                "\t-u | --underling-price      : Price of underlying asset [required]\n"
                "\t-s | --strike-price         : Strike price of the options contract [required]\n"
                "\t-t | --time-to-expiry       : Time to next (front) expiry date of the option contract "
                "in YYYY-mm-dd format [required]\n"
                "\t-v | --volatility           : Implied volatility of underlying asset [optional]\n"
                "\t-r | --rate-of-interest     : Risk Free interest rate [optional]\n\n"
                "\t(-h | --help                : Display this help/usage message)\n"
                "Optionally, if implied-volatility and interest-rate are omitted, the program will use "
                "default assumptions for these values.\n");
}

[[noreturn]] void helpAndExit(int code) {
    displayHelp();
    exit(code);
}

auto main(int argc, char **argv) -> int {
    ArgParser parser;
    try {
        fmt::print("Running black scholes merton\n");

        const Args params { argv + 1, argv + argc };
        if (!parser.populateArgs(params)) {
            helpAndExit(EXIT_FAILURE);
        }

        auto optionValues = parser.getOptionValues();

        if (parser.getOptionType() == OptionType::Call) {
            Option<CallExecutor> call(std::move(optionValues));
            fmt::print("Call Option Value: {}\n", call());
        }
        else {
            Option<PutExecutor> put(std::move(optionValues));
            fmt::print("Put Option Value: {}\n", put());
        }
    }
    catch (const std::exception &e) {
        fmt::print("Exeception caught during bsm run:\n{}", e.what());
        helpAndExit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
