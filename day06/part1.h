#pragma once

#include <algorithm>
#include <cmath>
#include <fmt/ranges.h>
#include <ranges>
#include <span>
#include <spanstream>
#include <string_view>
#include <vector>

namespace part_one
{

constexpr static auto print(const auto& container) -> void
{
    for (const auto& value : container) {
        fmt::print("{}, ", value);
    }
    fmt::println("");
}

[[nodiscard]] constexpr auto get_result(const std::string_view document) -> int
{
    std::vector<int> times;
    std::vector<int> distances;

    std::ispanstream stream(document);

    auto drop_text = [&] {
        while (not std::isdigit(stream.peek())) {
            stream.ignore();
        }
    };

    // Parse the times
    drop_text();
    std::ranges::copy(std::views::istream<int>(stream), std::back_inserter(times));
    stream.clear();

    // Parse the distances
    drop_text();
    std::ranges::copy(std::views::istream<int>(stream), std::back_inserter(distances));
    stream.clear();

    return std::ranges::fold_left(std::views::zip(times, distances), 1,
                                  [&](const int result, const auto& record) {
        const auto time     = std::get<0>(record);
        const auto distance = std::get<1>(record);

        /*
          distance < hold_time * (time - hold_time)
          distance < hold_time * time - hold_time * hold_time
          hold_time * hold_time -  time * hold_time + distance < 0
          x*x + (-b)*x + c < 0 (is same as quadratic equation ax2 + bx + c = 0)

           x = [-b ± √(b2 - 4ac)]/2a
           x = [b ± √(b2 - 4c)]/2 (because a = 1 and b = -b)
           max = [time + √(time * time - 4 * distance)]/2
           max = [time - √(time * time - 4 * distance)]/2
        */
        const double max = (time + std::sqrt((time * time) - (4 * distance))) / (2);
        const double min = (time - std::sqrt((time * time) - (4 * distance))) / (2);

        /*
          for accounting for the inequality take
          floor of the next lower value from the maximum solution and
          ceiling of the next higher value from the minimum solution
          then substarct them and add 1
        */

        return result
               * (std::floor(std::nexttoward(max, min)) - std::ceil(std::nexttoward(min, max)) + 1);
    });
}

} // namespace part_one
