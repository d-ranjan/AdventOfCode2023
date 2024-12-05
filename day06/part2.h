#pragma once

#include <algorithm>
#include <cmath>
#include <fmt/ranges.h>
#include <ranges>
#include <string_view>

namespace part_two
{

constexpr static auto print(const auto& container) -> void
{
    for (const auto& value : container) {
        fmt::print("{}, ", value);
    }
    fmt::println("");
}

[[nodiscard]] constexpr static auto get_result(const std::string_view document) -> int64_t
{
    int64_t time {};
    int64_t distance {};

    constexpr auto is_digit = [](const char ch) { return isdigit(ch) != 0; };

    for (const auto& line : document | std::views::split('\n')) {
        auto       str = line | std::views::filter(is_digit);
        const auto num = std::stoll(std::string(str.begin(), str.end()));
        time == 0 ? time = num : distance = num;
    }

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

    return std::floor(std::nexttoward(max, min)) - std::ceil(std::nexttoward(min, max)) + 1;
}

} // namespace part_two
