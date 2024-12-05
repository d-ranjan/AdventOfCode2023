#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <ranges>
#include <string>
#include <vector>

namespace part_one
{

[[nodiscard]] auto get_result(const std::string_view document) -> int
{
    constexpr auto is_not_digit = [](const char ch) { return isdigit(ch) == 0; };
    // Treat the first element of a range of character as a decimal digit
    constexpr auto to_int = [](auto&& rng) -> int { return *rng.begin() - '0'; };

    constexpr auto first_digit = std::views::drop_while(is_not_digit);
    constexpr auto last_digit  = std::views::reverse | std::views::drop_while(is_not_digit);

    int sum_of_calibration_value {};
    // Accumulate the calibration values
    for (const auto& line : std::views::split(document, '\n')) {
        sum_of_calibration_value += to_int(line | first_digit) * 10 + to_int(line | last_digit);
    }
    return sum_of_calibration_value;
}

} // namespace part_one
