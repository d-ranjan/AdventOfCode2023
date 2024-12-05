#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <ranges>
#include <spanstream>
#include <string_view>
#include <vector>

namespace part_two
{

[[nodiscard]] constexpr static auto calc_hash(auto&& steps) -> int
{
    return std::ranges::fold_left(steps, 0,
                                  [&](int sum, auto&& ch) { return ((sum + ch) * 17) % 256; });
}

struct Step
{
    int  slot_number {};
    std::string label;

    char operation;
    int  focal_length {};

    friend std::istream& operator>>(std::istream& stream, Step& step)
    {
        while (std::isalpha(stream.peek()) != 0) {
            step.label.push_back(stream.get());
        }

        return stream >> step.operation >> step.focal_length;
    }
};

[[nodiscard]] constexpr static auto get_result(const std::string_view document) -> int
{
    std::vector<std::vector<Step>> boxes(256);

    for (const auto line : document | std::views::split(',')) {
        std::ispanstream stream(line);

        Step curr_step {};
        stream >> curr_step;

        auto* const box = &boxes.at(calc_hash(curr_step.label));

        const auto itr = std::ranges::find_if(
            *box, [&](auto&& step) { return curr_step.label == step.label; });

        if (itr != box->end()) {
            if (curr_step.operation == '=') {
                itr->focal_length = curr_step.focal_length;
            } else {
                box->erase(itr);
            }
        } else if (curr_step.operation == '=') {
            box->push_back(curr_step);
        }
    }

    int total_focusing_power {};
    for (const auto& [box_index, box] : boxes | std::views::enumerate) {
        for (const auto& [slot_index, step] : box | std::views::enumerate) {
            total_focusing_power += (box_index + 1) * (slot_index + 1) * step.focal_length;
        }
    }

    return total_focusing_power;
}

} // namespace part_two
