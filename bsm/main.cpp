#include <cstdlib>
#include <fmt/core.h>

#include "arg_parser.h"
#include "constants.h"
#include "input_reader.h"
#include "options.h"

using namespace bsm;

void displayHelp() {
    fmt::print("BSM can be run by providing csv format via standard input; or"
                "Direct run of BSM command requires at least 4 of the required arguments:\n"
                "\t-o | --option-type          : Type of option ('call' or 'put') [required]\n"
                "\t-u | --underling-price      : Price of underlying asset [required]\n"
                "\t-s | --strike-price         : Strike price of the options contract [required]\n"
                "\t-t | --time-to-expiry       : Time to next (front) expiry date of the option contract "
                "in YYYY-mm-dd format [required]\n"
                "\t-v | --volatility           : Implied volatility of underlying asset [optional]\n"
                "\t-r | --rate-of-interest     : Risk Free interest rate [optional]\n\n"
                "\t-d | --dividend-yield       : Dividend yield rate [optional]\n\n"
                "\t(-h | --help                : Display this help/usage message)\n"
                "Optionally, if volatility, interest-rate, or dividend yield are omitted, the program will use "
                "default assumptions for these values.\n");
}

[[noreturn]] void helpAndExit(int code) {
    displayHelp();
    exit(code);
}

template <typename value_type = double>
void optionRun(OptionType type, OptionValues<value_type> &&values, const bool multi) {
    std::string_view delim = "\n";
    if (multi) {
        delim = " ";
    }

    if (type == OptionType::Call) {
        Option<CallExecutor> call(std::move(values));
        fmt::print("Call Option Value: {:.2f}{}", call(), delim);
        call.printGreeks(multi);
    }
    else if (type == OptionType::Put){
        Option<PutExecutor> put(std::move(values));
        fmt::print("Put Option Value: {:.2f}{}", put(), delim);
        put.printGreeks(multi);
    }
}

auto main(int argc, char **argv) -> int {
    ArgParser parser;
    try {
        fmt::print("Running black scholes merton\n");

        if (argc < 2) {
            InputReader reader(Format::CSV);
            for (std::string stdin; std::getline(std::cin, stdin); ) {
                if (!stdin.empty()) {
                    auto [type, optionValues] = reader.getOptionValues(stdin);
                    if (optionValues) {
                        optionRun(type, std::move(optionValues.value()), true);
                    }
                }
            }
        }
        else {
            const Args params { argv + 1, argv + argc };
            if (!parser.populateArgs(params)) {
                helpAndExit(EXIT_FAILURE);
            }

            auto optionValues = parser.getOptionValues();
            const auto type = parser.getOptionType();
            optionRun(type, std::move(optionValues), false);
        }
    }
    catch (const std::exception &e) {
        fmt::print("Exeception caught during bsm run:\n{}\n", e.what());
        helpAndExit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
