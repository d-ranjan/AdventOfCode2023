#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <ranges>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace part_one
{

struct map_value
{
    int64_t dest_start;
    int64_t source_start;
    int64_t length;
};

constexpr auto get_map_value(const auto& line) -> map_value
{
    std::vector<int64_t> values {};
    std::ranges::transform(
        std::views::split(line, ' '), std::back_inserter(values),
        [](auto&& value) { return std::stoll(std::string(value.begin(), value.end())); });

    return map_value {values.at(0), values.at(1), values.at(2)};
}

constexpr auto print(const auto& container) -> void
{
    for (const auto& value : container) {
        fmt::print("{}, ", value);
    }
    fmt::println("");
}

[[nodiscard]] constexpr auto get_result(const std::string_view document) -> int64_t
{
    std::vector<int64_t>       seeds {};
    std::vector<map_value> values {};

    std::unordered_map<int64_t, int64_t> seed_to_location;

    for (const auto& line : document | std::views::split('\n')) {
        // fmt::println("{}", std::string_view {line});

        if (line.empty()) {
            for (const auto& [seed, location] : seed_to_location) {
                bool is_value_maped {};

                for (const auto& value : values) {
                    if (!is_value_maped && (location >= value.source_start)
                        && (location < value.source_start + value.length)) {
                        seed_to_location.at(seed)
                            = value.dest_start + (location - value.source_start);
                        is_value_maped = true;
                    }
                }
            }
            // print(std::views::values(seed_to_location));
            values.clear();
        } else if (isdigit(line.front()) == 0) {
            if (line.front() == 's') {
                if (*(line.begin() + 4) == 's') {
                    for (auto&& str :
                         line | std::views::drop_while([](const char ch) { return ch != ':'; })
                             | std::views::drop(2) | std::views::split(' ')) {
                        const int64_t seed = std::stoll(std::string(str.begin(), str.end()));
                        seeds.emplace_back(seed);
                        seed_to_location.emplace(seed, seed);
                    }
                }
            }
        } else {
            values.emplace_back(get_map_value(line));
        }
    }

    // fmt::println("\nlocation:    35, 86, 43, 80");

    return *std::ranges::min_element(std::views::values(seed_to_location));
}

} // namespace part_one
