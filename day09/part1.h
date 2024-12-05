#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <ranges>
#include <span>
#include <spanstream>
#include <string_view>
#include <vector>

namespace part_one
{

constexpr static auto print(auto&& view) -> void
{
    for (auto&& value : view) {
        fmt::print("{}, ", value);
    }
    fmt::println("");
}

[[nodiscard]] constexpr static auto get_result(const std::string_view document) -> int
{
    // Accumulate the values
    return std::ranges::fold_left(document | std::views::split('\n'), 0, [&](int sum, auto&& line) {
        std::vector<int> values;
        std::ispanstream stream(line);

        std::ranges::copy(std::views::istream<int>(stream), std::back_inserter(values));
        sum += values.back();

        constexpr auto reverse_minus = [](int x, int y) { return y - x; };

        bool is_tested {};
        while (not is_tested) {
            values = values | std::views::pairwise_transform(reverse_minus)
                     | std::ranges::to<std::vector<int>>();
            sum       += values.back();
            is_tested  = std::ranges::all_of(values, [](int x) { return x == 0; });
            // print(values);
        }

        return sum;
    });
}

} // namespace part_one
