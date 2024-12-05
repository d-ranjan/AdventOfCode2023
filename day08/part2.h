#pragma once

#include <cctype>
#include <numeric>
#include <fmt/ranges.h>
#include <spanstream>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace part_two
{

struct Graph
{
    enum Direction
    {
        Left,
        Right
    };
    std::string& step(const std::string& state, Direction direction)
    {
        if (direction == Left) {
            return map[state].first;
        } else { // if (direction == Right)
            return map[state].second;
        }
    }

    friend std::istream& operator>>(std::istream& s, Graph& graph)
    {
        auto drop_irrelevant = [&] {
            while (s && not std::isupper(s.peek())) {
                s.ignore();
            }
        };
        auto read_location = [&] {
            auto get = [&] { return static_cast<char>(s.get()); };
            return std::string {get(), get(), get()};
        };
        while (not s.eof()) {
            drop_irrelevant();
            const std::string key = read_location();
            drop_irrelevant();
            const std::string left = read_location();
            drop_irrelevant();
            const std::string right = read_location();

            graph.map.emplace(key, std::make_pair(left, right));
            // Is this a starting position?
            if (key.ends_with('A')) {
                graph.start.push_back(key);
            }
            drop_irrelevant();
        }
        return s;
    }

    std::vector<std::string> start;

    std::unordered_map<std::string, std::pair<std::string, std::string>> map;
};

[[nodiscard]] constexpr static auto get_result(const std::string_view document) -> int64_t
{
    std::ispanstream s(document);
    std::string      instructions;
    s >> instructions;
    Graph graph;
    s >> graph;

    auto direction = [](int64_t pos, std::string_view instr) {
        if (instr[pos % instr.size()] == 'L') {
            return Graph::Left;
        } else {
            return Graph::Right;
        }
    };

    int64_t result = 1;
    for (const auto& ghost : graph.start) {
        std::string current = ghost;
        int64_t     steps   = 0;
        while (not current.ends_with('Z')) {
            current = graph.step(current, direction(steps, instructions));
            ++steps;
        }
        // The end of the loop is aligned with the instructions
        if (steps % instructions.length() != 0) {
            throw std::runtime_error("Unaligned ghost");
        }
        // The ghost is on a proper loop if it returns to this position after steps
        const std::string state = current;
        for (int64_t i = 0; i < steps; ++i) {
            current = graph.step(current, direction(i, instructions));
        }
        if (current != state) {
            throw std::runtime_error("The ghost is not on a proper loop");
        }
        result = std::lcm(result, steps);
    }

    return result;
}

} // namespace part_two
