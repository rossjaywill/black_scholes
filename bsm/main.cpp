#include <cstdlib>
#include <fmt/core.h>

#include "arg_parser.h"
#include "constants.h"
#include "input_reader.h"
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
                "\t-d | --dividend-yield       : Dividend yield rate [optional]\n\n"
                "\t(-h | --help                : Display this help/usage message)\n"
                "Optionally, if volatility, interest-rate, or dividend yield are omitted, the program will use "
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

        if (std::cin) {
            InputReader reader(Format::CSV);

            for (std::string stdin; std::getline(std::cin, stdin); ) {
                auto [type, values] = reader.getOptionValues(stdin);
                if (type == OptionType::Call) {
                    fmt::print("underlying values passed to opt calc: {}\n", values.underlyingPrice_);
                    fmt::print("strike values passed to opt calc: {}\n", values.strikePrice_);
                    fmt::print("time values passed to opt calc: {}\n", values.timeToExpiry_);
                    fmt::print("vol values passed to opt calc: {}\n", values.volatility_);
                    fmt::print("risk values passed to opt calc: {}\n", values.riskFreeInterest_);
                    fmt::print("divid values passed to opt calc: {}\n", values.dividendYield_);
                    Option<CallExecutor> call(std::move(values));
                    fmt::print("Call Option Value: {:.2f} ", call());
                    call.printGreeks();
                }
                else if (type == OptionType::Put){
                    Option<PutExecutor> put(std::move(values));
                    fmt::print("Put Option Value: {:.2f}\n", put());
                    put.printGreeks();
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
            if (type == OptionType::Call) {
                Option<CallExecutor> call(std::move(optionValues));
                fmt::print("Call Option Value: {:.2f}\n", call());
                call.printGreeks();
            }
            else if (type == OptionType::Put) {
                Option<PutExecutor> put(std::move(optionValues));
                fmt::print("Put Option Value: {:.2f}\n", put());
                put.printGreeks();
            }
        }
    }

    catch (const std::exception &e) {
        fmt::print("Exeception caught during bsm run:\n{}\n", e.what());
        helpAndExit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
