#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <ranges>

namespace part_two
{

constexpr std::array<std::string_view, 9> word_digits
    = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

auto to_digit(const std::string_view str) -> int
{
    if (str.empty()) {
        return -1;
    }

    if (isdigit(str.front()) != 0) {
        return str.front() - '0';
    }

    // Find the digit which is prefix of our current string (if any)
    const auto* match = std::ranges::find_if(
        word_digits, [&](const auto& digit) { return str.starts_with(digit); });
    if (match == word_digits.end()) {
        return -1;
    }

    return std::distance(word_digits.begin(), match) + 1;
}

[[nodiscard]] auto get_result(const std::string_view document) -> int
{
    // Accumulate the calibration values
    return std::ranges::fold_left(document | std::views::split('\n'), 0z,
                                  [&](int sum_of_calibration_value, auto&& line) {
        int first = -1;
        int last  = -1;

        for (const auto pos : std::views::iota(line.begin(), line.end())) {
            // Current suffix
            const auto substr = std::string_view(pos, line.end());

            // Check if it begins with a digit and update first and last
            if (const int digit = to_digit(substr); digit != -1) {
                if (first == -1) {
                    first = digit;
                }
                last = digit;
            }
        }
        return sum_of_calibration_value + first * 10 + last;
    });
}
} // namespace part_two
