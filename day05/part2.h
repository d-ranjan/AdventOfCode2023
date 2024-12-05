#pragma once

#include <algorithm>
#include <ranges>
#include <span>
#include <spanstream>
#include <vector>

namespace part_two
{

struct Range
{
    int64_t source_start;
    int64_t dest_start;
    int64_t size;

    bool operator<(const Range& other) const { return source_start < other.source_start; }

    friend std::istream& operator>>(std::istream& stream, Range& rng)
    {
        return stream >> rng.dest_start >> rng.source_start >> rng.size;
    }
};

struct SeedRange
{
    int64_t start;
    int64_t size;

    bool operator<(const SeedRange& other) const { return start < other.start; }

    friend std::istream& operator>>(std::istream& stream, SeedRange& rng)
    {
        return stream >> rng.start >> rng.size;
    }
};

struct Mapper
{
    std::vector<SeedRange> seeds;
    std::vector<Range>     seed_to_soil;
    std::vector<Range>     soil_to_fertilizer;
    std::vector<Range>     fertilizer_to_water;
    std::vector<Range>     water_to_light;
    std::vector<Range>     light_to_temperature;
    std::vector<Range>     temperature_to_humidity;
    std::vector<Range>     humidity_to_location;

    [[nodiscard]] constexpr static auto convert(const std::vector<SeedRange>& input,
                                                const std::vector<Range>&     map)
        -> std::vector<SeedRange>
    {
        std::vector<SeedRange> result;

        // Get the first relevant map
        auto it = std::ranges::upper_bound(
            map, Range {.source_start = input.front().start, .dest_start = 0, .size = 0},
            std::less<> {});
        if (it != map.begin()) {
            it = std::prev(it);
        }

        // For each seed range in the input (the ranges are already sorted)
        for (auto [start, size] : input) {
            while (size > 0) {
                if (it == map.end()) {
                    // No conversion, no more mappings
                    result.push_back({start, size});
                    size = 0;
                } else if (start < it->source_start) {
                    // No conversion (initial part of the range not covered by a mapping)
                    const int64_t actual = std::min(size, it->source_start - start);
                    result.push_back({start, actual});
                    start += actual;
                    size  -= actual;
                } else if (start - it->source_start >= it->size) {
                    // The current mapping is no longer relevant
                    ++it;
                } else {
                    // Actual conversion
                    const int64_t actual = std::min((it->source_start + it->size) - start, size);
                    result.push_back({start - it->source_start + it->dest_start, actual});
                    start += actual;
                    size  -= actual;
                }
            }
        }

        std::ranges::sort(result, std::less<> {});
        return result;
    }

    [[nodiscard]] constexpr auto all_seed_locations() const -> std::vector<SeedRange>
    {
        auto soil        = convert(seeds, seed_to_soil);
        auto fertilizer  = convert(soil, soil_to_fertilizer);
        auto water       = convert(fertilizer, fertilizer_to_water);
        auto light       = convert(water, water_to_light);
        auto temperature = convert(light, light_to_temperature);
        auto humidity    = convert(temperature, temperature_to_humidity);
        return convert(humidity, humidity_to_location);
    }

    friend std::istream& operator>>(std::istream& stream, Mapper& mapper)
    {
        std::string dump;
        stream >> dump;

        auto drop_text = [&] {
            while (not std::isdigit(stream.peek())) {
                stream.ignore();
            }
        };

        // Parse the seeds
        std::ranges::copy(std::views::istream<SeedRange>(stream), std::back_inserter(mapper.seeds));
        stream.clear();
        drop_text();

        // Parse the seed-to-soil map
        std::ranges::copy(std::views::istream<Range>(stream),
                          std::back_inserter(mapper.seed_to_soil));
        stream.clear();
        drop_text();

        // Parse the soil-to-fertilizer map
        std::ranges::copy(std::views::istream<Range>(stream),
                          std::back_inserter(mapper.soil_to_fertilizer));
        stream.clear();
        drop_text();

        // Parse the fertilizer-to-water map
        std::ranges::copy(std::views::istream<Range>(stream),
                          std::back_inserter(mapper.fertilizer_to_water));
        stream.clear();
        drop_text();
        // Parse the water-to-light map
        std::ranges::copy(std::views::istream<Range>(stream),
                          std::back_inserter(mapper.water_to_light));
        stream.clear();
        drop_text();

        // Parse the light_to_temperature map
        std::ranges::copy(std::views::istream<Range>(stream),
                          std::back_inserter(mapper.light_to_temperature));
        stream.clear();
        drop_text();

        // Parse the temperature-to-humidity map
        std::ranges::copy(std::views::istream<Range>(stream),
                          std::back_inserter(mapper.temperature_to_humidity));
        stream.clear();
        drop_text();

        // Parse the humidity-to-location map
        std::ranges::copy(std::views::istream<Range>(stream),
                          std::back_inserter(mapper.humidity_to_location));
        stream.clear();

        // Sort all the maps
        std::ranges::sort(mapper.seeds, std::less<> {});
        std::ranges::sort(mapper.seed_to_soil, std::less<> {});
        std::ranges::sort(mapper.soil_to_fertilizer, std::less<> {});
        std::ranges::sort(mapper.fertilizer_to_water, std::less<> {});
        std::ranges::sort(mapper.water_to_light, std::less<> {});
        std::ranges::sort(mapper.light_to_temperature, std::less<> {});
        std::ranges::sort(mapper.temperature_to_humidity, std::less<> {});
        std::ranges::sort(mapper.humidity_to_location, std::less<> {});
        return stream;
    }
};

[[nodiscard]] constexpr static auto get_result(const std::string_view document) -> int64_t
{
    std::ispanstream stream(document);

    Mapper map;
    if (not(stream >> map)) {
        throw std::runtime_error("Failed to parse");
    }

    return map.all_seed_locations().front().start;
}

} // namespace part_two
