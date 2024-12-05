#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <ranges>
#include <string_view>

namespace part_one
{

[[nodiscard]] auto get_result(const std::string_view document) -> int
{
    // Accumulate the values
    return std::ranges::fold_left(document | std::views::split('\n'), 0,
                                  [&](int sum, auto&& line) {
        const auto colon = std::ranges::find(line, ':');
        const auto sep = std::ranges::find(line, '|');

        const auto winner_cards = std::string_view(colon + 1, sep);
        const auto game_cards = std::string_view(sep + 1, line.end());

       // fmt::println("{}", winner_cards);
       // fmt::println("{}", game_cards);

        return sum + std::ranges::fold_left(std::views::chunk(game_cards, 3), 0,
                                      [&](int card_point, auto&& value){
            if(std::ranges::contains_subrange(winner_cards, value)){
                return card_point == 0 ? 1 : card_point * 2;
            }
            return card_point;
        });
    });
}

} // namespace part_one
