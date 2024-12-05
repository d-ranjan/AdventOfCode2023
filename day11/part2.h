#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <fmt/ranges.h>
#include <ranges>
#include <string_view>
#include <vector>

namespace part_two
{

constexpr int64_t expansion_factor = 1000000;

struct galaxy
{
    int64_t number;
    int64_t row;
    int64_t col;
};

[[nodiscard]] constexpr static auto get_result(const std::string_view document) -> int64_t
{
    std::vector<galaxy> galaxies;

    int64_t galaxy_num {};
    int64_t filler_row_count {};
    int64_t col_count {};
    for (const auto& [row, line] : document | std::views::split('\n') | std::views::enumerate) {
        if (not std::ranges::contains(line, '#')) {
            filler_row_count += expansion_factor - 1;
        }
        col_count = 0;
        for (const auto& ch : line) {
            if (ch == '#') {
                galaxies.emplace_back(galaxy {++galaxy_num, row + filler_row_count, col_count});
            }
            col_count++;
        }
    }

    std::vector<int64_t> filler_columns {};
    for (const auto& col : std::views::iota(0, col_count)) {
        if (std::ranges::none_of(galaxies, [&](const galaxy& gal) { return gal.col == col; })) {
            filler_columns.push_back(col);
        }
    }

    for (const auto& col : filler_columns | std::views::reverse) {
        for (auto& gal : galaxies) {
            if (gal.col > col) {
                gal.col += expansion_factor - 1;
            }
        }
    }

    int64_t       total_distance {};
    const int64_t galaxy_count = galaxies.size() + 1;
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
