#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <ranges>
#include <string>
#include <vector>

namespace part_one
{

struct Game
{
    int id;
    int max_blue_count;
    int max_green_count;
    int max_red_count;
};

[[nodiscard]] constexpr auto split(const std::string_view source, const std::string_view delimiter)
    -> std::vector<std::string_view>
{
    std::vector<std::string_view> result;

    for (const auto& token : std::views::split(source, delimiter)) {
        result.emplace_back(std::string_view {token.begin(), token.end()});
    }

    return result;
}

[[nodiscard]] constexpr auto parse_game(const std::string_view line) -> Game
{
    /******************************************************************************
    Game 1: 14 green, 8 blue, 9 red; 5 blue, 4 green, 2 red; 4 red, 4 blue, 4 green;
    1 blue, 3 green, 2 red; 10 red, 3 blue, 15 green; 2 red, 6 green, 3 blue
    *******************************************************************************/
    Game game {};

    const auto index = line.find(':');
    if (index != std::string_view::npos) {
        game.id = std::stoi(std::string(line.substr(5, index - 5)));
    }

    const auto attempts = split(std::views::drop(line, index + 2), "; ");
    for (const auto& attempt : attempts) {
        const auto squares = split(attempt, ", ");
        for (const auto& square : squares) {
            const auto color_index = square.find(' ');
            if (color_index != std::string_view::npos) {
                const auto value = std::stoi(std::string(square.substr(0, color_index)));
                switch (square.at(color_index + 1)) {
                    case 'r': {
                        if (game.max_red_count < value) {
                            game.max_red_count = value;
                        }
                        break;
                    }
                    case 'g': {
                        if (game.max_green_count < value) {
                            game.max_green_count = value;
                        }
                        break;
                    }
                    case 'b': {
                        if (game.max_blue_count < value) {
                            game.max_blue_count = value;
                        }
                        break;
                    }
                }
            }
        }
    }
    // fmt::println("{}: r={} g={} b={}", game.id, game.max_red_count, game.max_green_count,
    //              game.max_blue_count);
    return game;
}

[[nodiscard]] auto get_result(const std::string_view document) -> int
{
    // Accumulate the values
    return std::ranges::fold_left(std::views::split(document, '\n'), 0, [&](int sum, auto&& line) {
        // fmt::println("{}", std::string_view {line});
        const auto game = parse_game(std::string_view {line});

        if ((game.max_red_count <= 12) && (game.max_green_count <= 13)
            && (game.max_blue_count <= 14)) {
            return sum + game.id;
        }
        return sum;
    });
}

} // namespace part_one
