#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <fmt/ranges.h>
#include <ranges>
#include <string_view>
#include <vector>

namespace part_one
{

struct galaxy
{
    int number;
    int row;
    int col;
};

[[nodiscard]] constexpr static auto get_result(const std::string_view document) -> int
{
    std::vector<galaxy> galaxies;

    int galaxy_num {};
    int filler_row_count {};
    int col_count {};
    for (const auto& [row, line] : document | std::views::split('\n') | std::views::enumerate) {
        if (not std::ranges::contains(line, '#')) {
            filler_row_count++;
        }
        col_count = 0;
        for (const auto& ch : line) {
            if (ch == '#') {
                const int new_row = row + filler_row_count;
                galaxies.emplace_back(galaxy {++galaxy_num, new_row, col_count});
            }
            col_count++;
        }
    }

    std::vector<int> filler_columns {};
    for (const auto& col : std::views::iota(0, col_count)) {
        if (std::ranges::none_of(galaxies, [&](const galaxy& gal) { return gal.col == col; })) {
            filler_columns.push_back(col);
        }
    }

    for (const auto& col : filler_columns | std::views::reverse) {
        for (auto& gal : galaxies) {
            if (gal.col > col) {
                gal.col++;
            }
        }
    }

    int       total_distance {};
    const int galaxy_count = galaxies.size() + 1;
    for (auto [start, end] : std::views::cartesian_product(std::views::iota(1, galaxy_count),
                                                           std::views::iota(1, galaxy_count))) {
        if (start != end && start < end) {
            const auto gal_start = galaxies.at(start - 1);
            const auto gal_end   = galaxies.at(end - 1);
            if (gal_end.row != gal_start.row) {
                total_distance += std::abs(gal_end.row - gal_start.row);
            }
            if (gal_end.col != gal_start.col) {
                total_distance += std::abs(gal_end.col - gal_start.col);
            }
            // fmt::print("({}[{}][{}],", start, gal_start.row, gal_start.col);
            // fmt::print(" {}[{}][{}]) ", end, gal_end.row, gal_end.col);
            // fmt::println("{}",total_distance);
        }
    }
    // for (auto& gal : galaxies) {
    //     fmt::println("{} [{}][{}]", gal.number, gal.row, gal.col);
    // }

    return total_distance;
}

} // namespace part_one
