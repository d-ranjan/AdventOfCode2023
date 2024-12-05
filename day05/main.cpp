#include "input.h"
#include "part1.h"
#include "part2.h"

#include <chrono>
#include <fmt/chrono.h>

void test_equal(const auto result, const auto exp_value)
{
    if (result != exp_value) {
        fmt::println("FAIL: result({}) != exp_value({})", result, exp_value);
        return;
    }
    fmt::println("result = {}", result);
}

int main()
{
    const auto start {std::chrono::steady_clock::now()};

    test_equal(part_one::get_result(input::test), 35);
    test_equal(part_one::get_result(input::data), 388071289);
    test_equal(part_two::get_result(input::test), 46);
    test_equal(part_two::get_result(input::data), 84206669);

    const auto end {std::chrono::steady_clock::now()};
    const auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    fmt::println("elapsed_time: {}", elapsed_time);
}
