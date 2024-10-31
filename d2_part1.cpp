#include <algorithm>
#include <chrono>
#include <fstream>
#include <print>
#include <ranges>
#include <string>

struct Game
{
    int id;
    int blue_square_count;
    int green_square_count;
    int red_square_count;
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

[[nodiscard]] constexpr auto get_game(const std::string_view line) -> Game
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
                    case 'b': {
                        if (game.blue_square_count < value) {
                            game.blue_square_count = value;
                        }
                        break;
                    }
                    case 'g': {
                        if (game.green_square_count < value) {
                            game.green_square_count = value;
                        }
                        break;
                    }
                    case 'r': {
                        if (game.red_square_count < value) {
                            game.red_square_count = value;
                        }
                        break;
                    }
                }
            }
        }
    }

    return game;
}

void process_file(const std::string& file_path)
{
    std::ifstream file;

    file.open(file_path);

    if (!file.is_open()) {
        std::println("unable to open file.");
        return;
    }

    int sum_of_ids_of_possible_games {};

    std::string line {};
    while (std::getline(file, line)) {
        const auto game = get_game(line);

        if ((game.blue_square_count <= 14) && (game.green_square_count <= 13)
            && (game.red_square_count <= 12)) {
            sum_of_ids_of_possible_games += game.id;
        }

        // std::println("ID:{} B:{} G:{} R:{}", game.id, game.blue_square_count,
        //              game.green_square_count, game.red_square_count);
    }
    std::println("sum_of_ids_of_possible_games:{}", sum_of_ids_of_possible_games);
}

int main()
{
    using namespace std::string_view_literals;

    const auto start {std::chrono::steady_clock::now()};
    process_file("/root/Desktop/AdventOfCode2023/d2_input.txt");

    const auto end {std::chrono::steady_clock::now()};
    const auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::println("elapsed_time: {}", elapsed_time);
}
