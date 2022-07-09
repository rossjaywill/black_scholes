#ifndef INPUT_READER_H
#define INPUT_READER_H

#include "helpers.h"
#include "options.h"

#include <charconv>
#include <optional>
#include <vector>

namespace bsm
{

constexpr const auto CSV_COLUMNS = 7;
constexpr const auto CSV_TIME_COLUMN = 3;

enum class Format
{
    CSV,
    None,
};

template <typename value_type = double>
class InputReader
{
public:
    using OptionInput = std::pair<OptionType, std::optional<OptionValues<value_type>>>;

    InputReader() = delete;
    explicit InputReader(Format fmt) : fmt_(fmt), skipHeader_(true) {}

    constexpr auto getOptionValues(std::string_view line) -> OptionInput {
        OptionType type = OptionType::None;
        if(!validate(line)) {
            return { type, std::nullopt };
        }
        switch (fmt_) {
        case Format::CSV:
            return getValuesFromCsv(line);
        default:
            return { type, std::nullopt };
        }
    }

private:
    constexpr auto validate(std::string_view line) -> bool {
        switch (fmt_) {
        case Format::CSV:
            return validateCsv(line);
        default:
            return false;
        }
    }

    constexpr auto validateCsv(std::string_view line) -> bool {
        if (line.find("option_type") != std::string::npos) {
            return false;
        }
        return true;
    }

    constexpr auto getValuesFromCsv(std::string_view line) -> OptionInput {
        size_t pos = 0, count = 0;
        OptionType type = OptionType::Call;
        std::vector<value_type> parsed;
        while ((pos = line.find_first_of(',')) != std::string::npos) {
            std::string_view field = line.substr(0, std::min(pos, std::string::npos));
            if (!count) {
                type = (field.starts_with("p")) ? OptionType::Put : OptionType::Call;
            }

            value_type value = 0;
            if (count != CSV_TIME_COLUMN) {
                std::from_chars(field.data(), field.data() + field.size(), value);
                parsed.push_back(value);
            }
            else {
                parsed.push_back(parseDate(field));
            }

            line.remove_prefix(std::min(pos + 1, std::string::npos));
            ++count;
        }

        if (count != CSV_COLUMNS) {
            return { type, std::nullopt };
        }

        // TODO RJW: Find a neater way to forward these args
        OptionValues values { parsed[1], parsed[2], parsed[3], parsed[4], parsed[5], parsed[6] };
        return { type, values };
    }

    Format fmt_;
    bool skipHeader_;
};

}

#endif
