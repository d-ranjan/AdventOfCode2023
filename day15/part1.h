#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <ranges>
#include <string_view>

namespace part_one
{

[[nodiscard]] constexpr static auto calc_hash(auto&& steps) -> int
{
    return std::ranges::fold_left(steps, 0,
                                  [&](int sum, auto&& ch) { return ((sum + ch) * 17) % 256; });
}

[[nodiscard]] constexpr static auto get_result(const std::string_view document) -> int
{
    // Accumulate the values
    return std::ranges::fold_left(document | std::views::split('\n'), 0, [&](int sum, auto&& line) {
        // fmt::println("{}", std::string_view {line});

        return sum
               + std::ranges::fold_left(line | std::views::split(','), 0,
                                        [&](int current_value, auto&& steps) {
            return current_value + calc_hash(steps);
        });
    });
}

} // namespace part_one
