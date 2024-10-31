#include <fstream>
#include <print>
#include <string>

[[nodiscard]] constexpr int get_calibration_value(const std::string_view line)
{
    int first_digit = -1;
    int second_digit {};

    for (const auto& ch : line) {
        if (isdigit(ch)) {
            second_digit = ch - '0';
            if (first_digit < 0) {
                first_digit = second_digit;
            }
        }
    }
    return first_digit * 10 + second_digit;
}

[[nodiscard]] int get_sum_of_calibration_values(const std::string& file_path)
{
    std::fstream file;

    file.open(file_path, std::ios::in);

    if (!file.is_open()) {
        std::println("unable to open file.");
        return 0;
    }

    int         sum_of_calibration_values {};
    std::string line {};
    while (std::getline(file, line)) {
        sum_of_calibration_values += get_calibration_value(line);
    }
    return sum_of_calibration_values;
}

int main()
{
    static_assert(get_calibration_value("ghdh4hssj7nn") == 47);
    const auto result = get_sum_of_calibration_values("/root/Desktop/AVC/d1_input.txt");
    std::println("{}", result);
}
