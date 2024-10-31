#include <algorithm>
#include <chrono>
#include <fstream>
#include <print>
#include <ranges>
#include <string>

constexpr std::array<std::string, 9> word_digits {"one", "two",   "three", "four", "five",
                                                  "six", "seven", "eight", "nine"};

[[nodiscard]] constexpr int get_calibration_value(const std::string_view line)
{
    int first_digit = -1;
    int last_digit {};
    int first_digit_pos = -1;
    int last_digit_pos  = -1;

    for (int i = 0; i < line.size(); ++i) {
        const auto ch = line.at(i);
        if (isdigit(ch)) {
            last_digit     = ch - '0';
            last_digit_pos = i;
            if (first_digit < 0) {
                first_digit     = last_digit;
                first_digit_pos = i;
            }
        }
    }

    for (int i = 0; i < word_digits.size(); ++i) {
        const auto word_digit = word_digits.at(i);

        const auto first_index = line.find(word_digit);
        if (first_index != std::string_view::npos) {
            if (first_digit_pos < 0) { // no digits in the line
                first_digit     = i + 1;
                last_digit      = i + 1;
                first_digit_pos = first_index;
                last_digit_pos  = first_index;
            }

            if (first_index < first_digit_pos) {
                first_digit     = i + 1;
                first_digit_pos = first_index;
            }
        }

        const auto last_index = line.rfind(word_digit);
        if (last_index != std::string_view::npos) {
            if (last_index > last_digit_pos) {
                last_digit     = i + 1;
                last_digit_pos = last_index;
            }
        }
    }

    return first_digit * 10 + last_digit;
}

[[nodiscard]] int get_sum_of_calibration_values(const std::string& file_path)
{
    std::ifstream file;

    file.open(file_path);

    if (!file.is_open()) {
        std::println("unable to open file.");
        return 0;
    }

    int         sum_of_calibration_values {};
    std::string line {};
    while (std::getline(file, line)) {
        const auto calibration_value  = get_calibration_value(line);
        sum_of_calibration_values    += calibration_value;
    }
    return sum_of_calibration_values;
}

int main()
{
    using namespace std::string_view_literals;
    static_assert(get_calibration_value("4skbhsbtqc"sv) == 44, "sigle numeric_digit at start");
    static_assert(get_calibration_value("ghdhhssjnn9"sv) == 99, "sigle numeric_digit at end");
    static_assert(get_calibration_value("ghdhhssj7nn"sv) == 77, "sigle numeric_digit in middle");
    static_assert(get_calibration_value("sevenghdhhssjnn"sv) == 77, "sigle word_digit at start");
    static_assert(get_calibration_value("ghdhhssjnnfive"sv) == 55, "sigle word_digit at end");
    static_assert(get_calibration_value("ghdhhssjonen"sv) == 11, "single word_digit in middle");
    static_assert(get_calibration_value("3five4s84four9rtbzllggz"sv) == 39,
                  "start: numeric_digit & end: numeric_digit");
    static_assert(get_calibration_value("75sevenzdrpkv1onetwo"sv) == 72,
                  "start: numeric_digit & end: word_digit");
    static_assert(get_calibration_value("mxmkjvgsdzfhseightonetwoeight7"sv) == 87,
                  "start: word_digit & end: numeric_digit");
    static_assert(get_calibration_value("gsadjsixsix6njhqrnine"sv) == 69,
                  "start: word_digit & end: word_digit");
    static_assert(get_calibration_value("3369eightnine89"sv) == 39,
                  "multiple numeric_digit continusly");
    static_assert(get_calibration_value("gsixtwodh4hssj7nnthreeoneqq"sv) == 61,
                  "multiple word_digit continusly");
    static_assert(get_calibration_value("sixtwodh4hssj7nnthreeone"sv) == 61,
                  "multiple word_digit continusly");
    static_assert(get_calibration_value("xpqldjtonempcfseven8seven8one"sv) == 11,
                  "start & end same word digit");

    const auto start {std::chrono::steady_clock::now()};
    const auto result
        = get_sum_of_calibration_values("/root/Desktop/AdventOfCode2023/d1_input.txt");
    const auto end {std::chrono::steady_clock::now()};
    const auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::println("{}", result);
    std::println("elapsed_time: {}", elapsed_time);
}
