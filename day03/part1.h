#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <ranges>
#include <string>
#include <vector>


namespace part_one
{

 [[nodiscard]] auto get_result(const std::string_view document) -> int
{
    // Preprocess the input so we are working with a 2D matrix
    std::vector<std::string_view> lines;
    std::ranges::transform(document | std::views::split('\n'), std::back_inserter(lines),
                           [](auto&& line) { return std::string_view {line}; });

    // Helper that checks the position for special character and also handles
    // corner cases.
    auto isspecial = [&](int64_t row, int64_t col) {
        if (row < 0 || col < 0) {
            return false;
        }
        if (row >= std::ssize(lines) || col >= std::ssize(lines[row])) {
            return false;
        }
        return (std::isdigit(lines[row][col]) == 0) && (lines[row][col] != '.');
    };

    int sum = 0;
    for (int row {}; const auto& line : lines) {
        // Have we seen a special character on the previous column?
        bool last = false;

        // Currently parsed number
        int number = 0;
        // Is the number valid (i.e. it has a special character next to it)
        bool valid = false;

        for (int col {}; const auto ch : line) {
            // Is there a special character on the current column?
            const bool current
                = isspecial(row, col) || isspecial(row - 1, col) || isspecial(row + 1, col);

            // Parse the digit
            if (isdigit(ch) != 0) {
                valid  |= last || current;
                number *= 10;
                number += ch - '0';
            } else if (number != 0) {
                // We are done parsing, check the current column.
                valid |= current;
                // If the number is valid, add it to the total.
                if (valid) {
                    sum += number;
                }

                // Reset
                number = 0;
                valid  = false;
            }

            last = current;
            col++;
        }
        // Don't forget that we can also finish parsing by running into end of line.
        if (number != 0 && valid) {
            sum += number;
        }
        row++;
    }
    return sum;
}

} // namespace part_one
