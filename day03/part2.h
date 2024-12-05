#pragma once

#include <fmt/ranges.h>
#include <ranges>
#include <string>
#include <vector>

namespace part_two
{

struct number
{
    int         row;
    int         col;
    std::string value;
};

[[nodiscard]] auto get_result(const std::string_view document) -> int
{
    std::vector<std::pair<int, int>> gears;
    std::vector<std::vector<number>> numbers_store {};

    for (const auto& [row, line] : document | std::views::split('\n') | std::views::enumerate) {
        std::vector<number> numbers {};
        number              num {};
        bool                is_number_started {};
        for (const auto [col, ch] : line | std::views::enumerate) {
            if (std::isdigit(ch) != 0) {
                if (!is_number_started) {
                    num.row           = row;
                    num.col           = col;
                    is_number_started = true;
                }
                num.value.push_back(ch);
            } else {
                if (ch == '*') {
                    gears.emplace_back(row, col);
                }
                if (is_number_started) {
                    numbers.push_back(num);
                    num.value.clear();
                    is_number_started = false;
                }
            }
        }
        if (is_number_started) {
            numbers.push_back(num);
        }
        numbers_store.push_back(numbers);
    }

    int sum_of_gear_ratio {};

    for (const auto& [row, col] : gears) {
        int adjacent_number_count {};
        int gear_ratio {1};

        for (const auto& numbers : std::views::counted(numbers_store.begin() + row - 1, 3)) {
            for (const auto& num : numbers) {
                if ((num.col == col + 1)
                    || (num.col <= col) && (num.col + num.value.size() >= col)) {
                    gear_ratio *= std::stoi(num.value);
                    adjacent_number_count++;
                    // fmt::println("{}[{}]", num.value, adjacent_number_count);
                }
            }
        }
        sum_of_gear_ratio += adjacent_number_count == 2 ? gear_ratio : 0;
    }
    return sum_of_gear_ratio;
}

} // namespace part_two
