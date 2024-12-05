#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <fmt/ranges.h>
#include <ranges>
#include <span>
#include <spanstream>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace part_one
{

[[nodiscard]] constexpr static auto get_result(const std::string_view document) -> int
{
    int         sum {};
    std::string commands {};

    std::unordered_map<std::string, std::pair<std::string, std::string>> nodes;

    for (auto&& line : document | std::views::split('\n')) {
        if (commands.empty()) {
            commands = std::string_view {line};
        } else if (not line.empty()) {
            auto key   = std::string_view {std::views::counted(line.begin(), 3)};
            auto left  = std::string_view {std::views::counted(line.begin() + 7, 3)};
            auto right = std::string_view {std::views::counted(line.begin() + 12, 3)};

            nodes.emplace(key, std::make_pair(left, right));
        }
    }

    const std::string final_key {"ZZZ"};
    std::string       initial_key {"AAA"};

    int idx {};

    while (initial_key != final_key) {
        const auto& [left, right] = nodes.at(initial_key);

        initial_key = commands.at(idx % commands.size()) == 'L' ? left : right;
        idx++;
        sum++;
    }

    return sum;
}

} // namespace part_one
