#pragma once

#include <algorithm>
#include <fmt/ranges.h>
#include <ranges>
#include <span>
#include <spanstream>
#include <string_view>
#include <vector>

namespace part_two
{

struct Card
{
    enum class Type
    {
        Jack = 1,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Queen,
        King,
        Ace
    } value;

    // Make it comparable
    auto operator<=>(const Card& other) const = default;

    friend std::istream& operator>>(std::istream& s, Card& card)
    {
        while (std::isspace(s.peek()) != 0) {
            s.ignore();
        }
        switch (s.get()) {
            case 'J': card.value = Type::Jack; break;
            case '2': card.value = Type::Two; break;
            case '3': card.value = Type::Three; break;
            case '4': card.value = Type::Four; break;
            case '5': card.value = Type::Five; break;
            case '6': card.value = Type::Six; break;
            case '7': card.value = Type::Seven; break;
            case '8': card.value = Type::Eight; break;
            case '9': card.value = Type::Nine; break;
            case 'T': card.value = Type::Ten; break;
            case 'Q': card.value = Type::Queen; break;
            case 'K': card.value = Type::King; break;
            case 'A': card.value = Type::Ace; break;
            default: s.setstate(std::ios_base::failbit); return s;
        }
        return s;
    }
};

struct Hand
{
    std::array<Card, 5> cards;
    int                 bid;
    enum class Type
    {
        HighCard,
        OnePair,
        TwoPair,
        ThreeOfKind,
        FullHouse,
        FourOfKind,
        FiveOfKind
    } value;

    void analyze_hand()
    {
        std::array<Card, 5> sorted_cards = cards;

        std::ranges::sort(sorted_cards, std::greater<> {});

        // Translate cards into frequencies of equal cards
        std::array<int64_t, 5> freq = {1, 0, 0, 0, 0};

        int   joker_count{};
        auto* it = freq.begin();
        for (int i = 1; i < 5; ++i) {
            if (sorted_cards[i].value == Card::Type::Jack) {
                ++joker_count;
                continue;
            }
            if (sorted_cards[i - 1] != sorted_cards[i]) {
                ++it;
            }
            ++*it;
        }

        std::ranges::sort(freq, std::greater<> {});

        const auto max_freq        = freq.at(0) + joker_count;
        const auto second_max_freq = freq.at(1);

        // Map the frequencies to the hand type
        if (max_freq == 5) {
            value = Type::FiveOfKind;
        } else if (max_freq == 4) {
            value = Type::FourOfKind;
        } else if (max_freq == 3 && second_max_freq == 2) {
            value = Type::FullHouse;
        } else if (max_freq == 3 && second_max_freq != 2) {
            value = Type::ThreeOfKind;
        } else if (max_freq == 2 && second_max_freq == 2) {
            value = Type::TwoPair;
        } else if (max_freq == 2 && second_max_freq != 2) {
            value = Type::OnePair;
        } else {
            value = Type::HighCard;
        }
    }

    friend std::istream& operator>>(std::istream& stream, Hand& hand)
    {
        for (auto& e : hand.cards) {
            if (not(stream >> e)) {
                return stream;
            }
        }
        if (not(stream >> hand.bid)) {
            return stream;
        }
        hand.analyze_hand();
        return stream;
    }

    auto operator<=>(const Hand& other) const
    {
        // First compare by the hand value
        auto cmp = value <=> other.value;
        if (std::is_neq(cmp)) {
            return cmp;
        }
        // If the hands are the same, lexicographically compare the cards
        return cards <=> other.cards;
    }
};

[[nodiscard]] constexpr static auto get_result(const std::string_view document) -> int
{
    std::ispanstream  stream(document);
    std::vector<Hand> hands;

    std::ranges::copy(std::views::istream<Hand>(stream), std::back_inserter(hands));

    std::ranges::sort(hands, std::less<> {});

    return std::ranges::fold_left(hands | std::views::enumerate, 0, [](int acc, const auto& e) {
        const auto [idx, hand] = e;
        return acc + (hand.bid * (idx + 1));
    });
}

} // namespace part_two
