#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <ranges>
#include <vector>

namespace part_two
{

struct Card
{
    int count;
    int matches;
};

[[nodiscard]] constexpr auto process_card(auto&& line) -> Card
{
    const auto colon = std::ranges::find(line, ':');
    const auto sep   = std::ranges::find(line, '|');

    const auto winner_cards = std::string_view(colon + 1, sep);
    const auto game_cards   = std::string_view(sep + 1, line.end());

    const auto match_count
        = std::ranges::count_if(game_cards | std::views::chunk(3), [&](auto&& value) {
        return std::ranges::contains_subrange(winner_cards, value);
    });
    return Card {1, static_cast<int>(match_count)};
}

constexpr auto print_view(const auto& range) -> void
{
    for (const int e : range) {
        fmt::print("{} ", e);
    }
    fmt::println("");
}

[[nodiscard]] constexpr auto get_result(const std::string_view document) -> int
{
    std::vector<Card> cards; // {count, matches}
    std::ranges::transform(document | std::views::split('\n'), std::back_inserter(cards),
                           [](auto&& line) { return process_card(line); });

    for (const auto& [row, card] : cards | std::views::enumerate) {
        for (size_t card_no = row + 1; card_no <= row + card.matches; ++card_no) {
            cards.at(card_no).count += card.count;
        }
    }

    return std::ranges::fold_left(cards | std::views::transform(&Card::count), 0, std::plus<> {});
}

} // namespace part_two
