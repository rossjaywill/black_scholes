#ifndef INPUT_READER_H
#define INPUT_READER_H

#include "helpers.h"
#include "options.h"

#include <charconv>
#include <vector>

namespace bsm
{

enum class Format
{
    CSV,
    None,
};

template <typename value_type = double>
class InputReader
{
public:
    using OptionRun = std::pair<OptionType, OptionValues<value_type>&>;

    InputReader() = delete;
    explicit InputReader(Format fmt) : fmt_(fmt), skipHeader_(true) {}

    auto getOptionValues(std::string_view line) -> OptionRun {
        OptionValues<value_type> values;
        OptionType type = OptionType::None;
        if(!validate(line)) {
            return { type, values };
        }
        switch (fmt_) {
        case Format::CSV:
            fmt::print("fmt is csv calling parse csv line\n");
            type = getValuesFromCsv(line, values);
            break;
        default:
            fmt::print("fmt is unspecified!!!\n");
            break;
        }
        fmt::print("get from opt values has underlying: {}\n", values.underlyingPrice_);
        return { type, values };
    }

private:
    auto validate(std::string_view line) -> bool {
        switch (fmt_) {
        case Format::CSV:
            return validateCsv(line);
        default:
            return false;
        }
    }

    auto validateCsv(std::string_view line) -> bool {
        if (line.find("option_type") != std::string::npos) {
            return false;
        }
        return true;
    }

    auto getValuesFromCsv(std::string_view line, OptionValues<value_type> &values) -> OptionType {
        size_t pos = 0, column = 0;
        OptionType type = OptionType::Call;
        while ((pos = line.find_first_of(',')) != std::string::npos) {
            std::string_view field = line.substr(0, std::min(pos, std::string::npos));
            fmt::print("iter with field: {}\n", field);
            fmt::print("iter count: {}\n", column);
            if (column < 10) {fmt::print("line iter with field: {}\n", field); }
            // TODO RJW: Refactor this to lookup table
            value_type value;
            switch (column) {
            case 0:
                type = (field.starts_with("p")) ? OptionType::Put : OptionType::Call;
                break;
            case 1:
                std::from_chars(field.data(), field.data() + field.size(), value);
                fmt::print("parse value of underlying as: {}\n", value);
                values.underlyingPrice_ = value;
                break;
            case 2:
                std::from_chars(field.data(), field.data() + field.size(), value);
                fmt::print("parse value of strike as: {}\n", value);
                values.strikePrice_ = value;
                break;
            case 3:
                fmt::print("parse value of time as: {}\n", field);
                values.timeToExpiry_ = parseDate(field);
                break;
            case 4:
                std::from_chars(field.data(), field.data() + field.size(), value);
                fmt::print("parse value of vol as: {}\n", value);
                values.volatility_ = value;
                break;
            case 5:
                std::from_chars(field.data(), field.data() + field.size(), value);
                fmt::print("parse value of interest as: {}\n", value);
                values.riskFreeInterest_ = value;
                break;
            case 6:
                fmt::print("parse value of dividend as: {}\n", value);
                std::from_chars(field.data(), field.data() + field.size(), value);
                values.dividendYield_ = value;
                break;
            }
            line.remove_prefix(std::min(pos + 1, std::string::npos));
            ++column;
        }
        fmt::print("get from csv line has underlying: {}\n", values.underlyingPrice_);
        return type;
    }

    Format fmt_;
    bool skipHeader_;
};

}

#endif
