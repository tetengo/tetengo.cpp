/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <clocale>
#include <iterator>
#include <locale>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/text/graphemeSplitter.h>
#include <tetengo/text/grapheme_splitter.hpp>


namespace
{
    std::optional<std::locale> make_locale_zh()
    {
        try
        {
#if _WIN32
            return std::locale{ "Chinese (Simplified)_China" };
#else
            return std::locale{ "zh_CN" };
#endif
        }
        catch (const std::runtime_error&)
        {
            BOOST_WARN_MESSAGE(false, "The locale zh_CN is not supported on your system.");
            return std::nullopt;
        }
    }

    std::optional<std::locale> make_locale_ja()
    {
        try
        {
#if _WIN32
            return std::locale{ "Japanese_Japan" };
#else
            return std::locale{ "ja_JP" };
#endif
        }
        catch (const std::runtime_error&)
        {
            BOOST_WARN_MESSAGE(false, "The locale ja_JP is not supported on your system.");
            return std::nullopt;
        }
    }

    std::optional<std::locale> make_locale_ko()
    {
        try
        {
#if _WIN32
            return std::locale{ "Korean_Korea" };
#else
            return std::locale{ "ko_KR" };
#endif
        }
        catch (const std::runtime_error&)
        {
            BOOST_WARN_MESSAGE(false, "The locale ko_KR is not supported on your system.");
            return std::nullopt;
        }
    }

    std::optional<std::locale> make_locale_en()
    {
        try
        {
#if _WIN32
            return std::locale{ "English_United States" };
#else
            return std::locale{ "en_US" };
#endif
        }
        catch (const std::runtime_error&)
        {
            BOOST_WARN_MESSAGE(false, "The locale en_US is not supported on your system.");
            return std::nullopt;
        }
    }

    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    // clang-format off
    const std::string string_iruka{
        0x4D_c,                 // U+004D
        0x79_c,                 // U+0079
        0x20_c,                 // U+0020
        0xE6_c, 0xB2_c, 0xB3_c, // U+6CB3
        0xE8_c, 0xB1_c, 0x9A_c, // U+8C5A
    };
    // clang-format on

    // clang-format off
    const std::string string_emoji_cyrillic_greek{
        0xE2_c, 0x98_c, 0xBA_c,         // U+263A
        0xF0_c, 0x9F_c, 0x91_c, 0xA9_c, // U+1F469
        0xE2_c, 0x80_c, 0x8D_c,         // U+200D
        0xE2_c, 0x9D_c, 0xA4_c,         // U+2764
        0xEF_c, 0xB8_c, 0x8F_c,         // U+FE0F
        0xE2_c, 0x80_c, 0x8D_c,         // U+200D
        0xF0_c, 0x9F_c, 0x92_c, 0x8B_c, // U+1F48B
        0xE2_c, 0x80_c, 0x8D_c,         // U+200D
        0xF0_c, 0x9F_c, 0x91_c, 0xA8_c, // U+1F468
        0xD0_c, 0x94_c,                 // U+0414
        0xCE_c, 0xB2_c,                 // U+03B2
    };
    // clang-format on


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(text)
BOOST_AUTO_TEST_SUITE(grapheme)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const tetengo::text::grapheme grapheme_{ 4, 2 };
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::text::grapheme grapheme1{ 4, 2 };
        const tetengo::text::grapheme grapheme2{ 4, 2 };

        BOOST_CHECK(grapheme1 == grapheme2);
    }
    {
        const tetengo::text::grapheme grapheme1{ 4, 2 };
        const tetengo::text::grapheme grapheme2{ 5, 3 };

        BOOST_CHECK(grapheme1 != grapheme2);
    }
}

BOOST_AUTO_TEST_CASE(offset)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::text::grapheme grapheme_{ 4, 2 };

    BOOST_TEST(grapheme_.offset() == 4U);
}

BOOST_AUTO_TEST_CASE(width)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::text::grapheme grapheme_{ 4, 2 };

    BOOST_TEST(grapheme_.width() == 2U);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(grapheme_splitter)
BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::text::grapheme_splitter splitter{};
    }
    if (auto o_locale = make_locale_zh(); o_locale)
    {
        const tetengo::text::grapheme_splitter splitter{ *o_locale };
    }
    if (auto o_locale = make_locale_ja(); o_locale)
    {
        const tetengo::text::grapheme_splitter splitter{ *o_locale };
    }
    if (auto o_locale = make_locale_ko(); o_locale)
    {
        const tetengo::text::grapheme_splitter splitter{ *o_locale };
    }
    if (auto o_locale = make_locale_en(); o_locale)
    {
        const tetengo::text::grapheme_splitter splitter{ *o_locale };
    }

    if (auto o_locale = make_locale_zh(); o_locale)
    {
        const std::string previous_locale_name{ std::setlocale(LC_CTYPE, nullptr) };
        std::setlocale(LC_CTYPE, o_locale->name().c_str());
        const auto* const p_grapheme_splitter = tetengo_text_graphemeSplitter_create();
        BOOST_SCOPE_EXIT(&previous_locale_name, p_grapheme_splitter)
        {
            tetengo_text_graphemeSplitter_destroy(p_grapheme_splitter);
            std::setlocale(LC_CTYPE, previous_locale_name.c_str());
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_grapheme_splitter);
    }
    if (auto o_locale = make_locale_ja(); o_locale)
    {
        const std::string previous_locale_name{ std::setlocale(LC_CTYPE, nullptr) };
        std::setlocale(LC_CTYPE, o_locale->name().c_str());
        const auto* const p_grapheme_splitter = tetengo_text_graphemeSplitter_create();
        BOOST_SCOPE_EXIT(&previous_locale_name, p_grapheme_splitter)
        {
            tetengo_text_graphemeSplitter_destroy(p_grapheme_splitter);
            std::setlocale(LC_CTYPE, previous_locale_name.c_str());
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_grapheme_splitter);
    }
    if (auto o_locale = make_locale_ko(); o_locale)
    {
        const std::string previous_locale_name{ std::setlocale(LC_CTYPE, nullptr) };
        std::setlocale(LC_CTYPE, o_locale->name().c_str());
        const auto* const p_grapheme_splitter = tetengo_text_graphemeSplitter_create();
        BOOST_SCOPE_EXIT(&previous_locale_name, p_grapheme_splitter)
        {
            tetengo_text_graphemeSplitter_destroy(p_grapheme_splitter);
            std::setlocale(LC_CTYPE, previous_locale_name.c_str());
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_grapheme_splitter);
    }
    if (auto o_locale = make_locale_en(); o_locale)
    {
        const std::string previous_locale_name{ std::setlocale(LC_CTYPE, nullptr) };
        std::setlocale(LC_CTYPE, o_locale->name().c_str());
        const auto* const p_grapheme_splitter = tetengo_text_graphemeSplitter_create();
        BOOST_SCOPE_EXIT(&previous_locale_name, p_grapheme_splitter)
        {
            tetengo_text_graphemeSplitter_destroy(p_grapheme_splitter);
            std::setlocale(LC_CTYPE, previous_locale_name.c_str());
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_grapheme_splitter);
    }
}

BOOST_AUTO_TEST_CASE(split)
{
    BOOST_TEST_PASSPOINT();

    if (auto o_locale = make_locale_zh(); o_locale)
    {
        const tetengo::text::grapheme_splitter splitter{ *o_locale };
        {
            const auto graphemes = splitter.split(string_iruka);
            BOOST_TEST_REQUIRE(std::size(graphemes) == 5U);
            BOOST_TEST((graphemes[0] == tetengo::text::grapheme{ 0, 1 }));
            BOOST_TEST((graphemes[1] == tetengo::text::grapheme{ 1, 1 }));
            BOOST_TEST((graphemes[2] == tetengo::text::grapheme{ 2, 1 }));
            BOOST_TEST((graphemes[3] == tetengo::text::grapheme{ 3, 2 }));
            BOOST_TEST((graphemes[4] == tetengo::text::grapheme{ 6, 2 }));
        }
        {
            const auto graphemes = splitter.split(string_emoji_cyrillic_greek);
            BOOST_TEST_REQUIRE(std::size(graphemes) == 4U);
            BOOST_TEST((graphemes[0] == tetengo::text::grapheme{ 0, 2 }));
            BOOST_TEST((graphemes[1] == tetengo::text::grapheme{ 3, 2 }));
            BOOST_TEST((graphemes[2] == tetengo::text::grapheme{ 30, 2 }));
            BOOST_TEST((graphemes[3] == tetengo::text::grapheme{ 32, 2 }));
        }
    }
    if (auto o_locale = make_locale_ja(); o_locale)
    {
        const tetengo::text::grapheme_splitter splitter{ *o_locale };
        {
            const auto graphemes = splitter.split(string_iruka);
            BOOST_TEST_REQUIRE(std::size(graphemes) == 5U);
            BOOST_TEST((graphemes[0] == tetengo::text::grapheme{ 0, 1 }));
            BOOST_TEST((graphemes[1] == tetengo::text::grapheme{ 1, 1 }));
            BOOST_TEST((graphemes[2] == tetengo::text::grapheme{ 2, 1 }));
            BOOST_TEST((graphemes[3] == tetengo::text::grapheme{ 3, 2 }));
            BOOST_TEST((graphemes[4] == tetengo::text::grapheme{ 6, 2 }));
        }
        {
            const auto graphemes = splitter.split(string_emoji_cyrillic_greek);
            BOOST_TEST_REQUIRE(std::size(graphemes) == 4U);
            BOOST_TEST((graphemes[0] == tetengo::text::grapheme{ 0, 2 }));
            BOOST_TEST((graphemes[1] == tetengo::text::grapheme{ 3, 2 }));
            BOOST_TEST((graphemes[2] == tetengo::text::grapheme{ 30, 2 }));
            BOOST_TEST((graphemes[3] == tetengo::text::grapheme{ 32, 2 }));
        }
    }
    if (auto o_locale = make_locale_ko(); o_locale)
    {
        const tetengo::text::grapheme_splitter splitter{ *o_locale };
        {
            const auto graphemes = splitter.split(string_iruka);
            BOOST_TEST_REQUIRE(std::size(graphemes) == 5U);
            BOOST_TEST((graphemes[0] == tetengo::text::grapheme{ 0, 1 }));
            BOOST_TEST((graphemes[1] == tetengo::text::grapheme{ 1, 1 }));
            BOOST_TEST((graphemes[2] == tetengo::text::grapheme{ 2, 1 }));
            BOOST_TEST((graphemes[3] == tetengo::text::grapheme{ 3, 2 }));
            BOOST_TEST((graphemes[4] == tetengo::text::grapheme{ 6, 2 }));
        }
        {
            const auto graphemes = splitter.split(string_emoji_cyrillic_greek);
            BOOST_TEST_REQUIRE(std::size(graphemes) == 4U);
            BOOST_TEST((graphemes[0] == tetengo::text::grapheme{ 0, 2 }));
            BOOST_TEST((graphemes[1] == tetengo::text::grapheme{ 3, 2 }));
            BOOST_TEST((graphemes[2] == tetengo::text::grapheme{ 30, 2 }));
            BOOST_TEST((graphemes[3] == tetengo::text::grapheme{ 32, 2 }));
        }
    }
    if (auto o_locale = make_locale_en(); o_locale)
    {
        const tetengo::text::grapheme_splitter splitter{ *o_locale };
        {
            const auto graphemes = splitter.split(string_iruka);
            BOOST_TEST_REQUIRE(std::size(graphemes) == 5U);
            BOOST_TEST((graphemes[0] == tetengo::text::grapheme{ 0, 1 }));
            BOOST_TEST((graphemes[1] == tetengo::text::grapheme{ 1, 1 }));
            BOOST_TEST((graphemes[2] == tetengo::text::grapheme{ 2, 1 }));
            BOOST_TEST((graphemes[3] == tetengo::text::grapheme{ 3, 2 }));
            BOOST_TEST((graphemes[4] == tetengo::text::grapheme{ 6, 2 }));
        }
        {
            const auto graphemes = splitter.split(string_emoji_cyrillic_greek);
            BOOST_TEST_REQUIRE(std::size(graphemes) == 4U);
            BOOST_TEST((graphemes[0] == tetengo::text::grapheme{ 0, 1 }));
            BOOST_TEST((graphemes[1] == tetengo::text::grapheme{ 3, 2 }));
            BOOST_TEST((graphemes[2] == tetengo::text::grapheme{ 30, 1 }));
            BOOST_TEST((graphemes[3] == tetengo::text::grapheme{ 32, 1 }));
        }
    }

    if (auto o_locale = make_locale_zh(); o_locale)
    {
        const std::string previous_locale_name{ std::setlocale(LC_CTYPE, nullptr) };
        std::setlocale(LC_CTYPE, o_locale->name().c_str());
        const auto* const p_grapheme_splitter = tetengo_text_graphemeSplitter_create();
        BOOST_SCOPE_EXIT(&previous_locale_name, p_grapheme_splitter)
        {
            tetengo_text_graphemeSplitter_destroy(p_grapheme_splitter);
            std::setlocale(LC_CTYPE, previous_locale_name.c_str());
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_grapheme_splitter);

        {
            const auto grapheme_count =
                tetengo_text_graphemeSplitter_split(p_grapheme_splitter, string_iruka.c_str(), nullptr);
            BOOST_TEST_REQUIRE(grapheme_count == 5U);
            std::vector<tetengo_text_grapheme_t> graphemes(grapheme_count);
            const auto                           grapheme_count_again =
                tetengo_text_graphemeSplitter_split(p_grapheme_splitter, string_iruka.c_str(), std::data(graphemes));
            BOOST_TEST(grapheme_count_again == grapheme_count);
            BOOST_TEST(graphemes[0].offset == 0U);
            BOOST_TEST(graphemes[0].width == 1U);
            BOOST_TEST(graphemes[1].offset == 1U);
            BOOST_TEST(graphemes[1].width == 1U);
            BOOST_TEST(graphemes[2].offset == 2U);
            BOOST_TEST(graphemes[2].width == 1U);
            BOOST_TEST(graphemes[3].offset == 3U);
            BOOST_TEST(graphemes[3].width == 2U);
            BOOST_TEST(graphemes[4].offset == 6U);
            BOOST_TEST(graphemes[4].width == 2U);
        }
        {
            const auto grapheme_count =
                tetengo_text_graphemeSplitter_split(p_grapheme_splitter, string_emoji_cyrillic_greek.c_str(), nullptr);
            BOOST_TEST_REQUIRE(grapheme_count == 4U);
            std::vector<tetengo_text_grapheme_t> graphemes(grapheme_count);
            const auto                           grapheme_count_again = tetengo_text_graphemeSplitter_split(
                p_grapheme_splitter, string_emoji_cyrillic_greek.c_str(), std::data(graphemes));
            BOOST_TEST(grapheme_count_again == grapheme_count);
            BOOST_TEST(graphemes[0].offset == 0U);
            BOOST_TEST(graphemes[0].width == 2U);
            BOOST_TEST(graphemes[1].offset == 3U);
            BOOST_TEST(graphemes[1].width == 2U);
            BOOST_TEST(graphemes[2].offset == 30U);
            BOOST_TEST(graphemes[2].width == 2U);
            BOOST_TEST(graphemes[3].offset == 32U);
            BOOST_TEST(graphemes[3].width == 2U);
        }
        {
            const auto grapheme_count = tetengo_text_graphemeSplitter_split(p_grapheme_splitter, nullptr, nullptr);
            BOOST_TEST(grapheme_count == 0U);
        }
    }
    if (auto o_locale = make_locale_ja(); o_locale)
    {
        const std::string previous_locale_name{ std::setlocale(LC_CTYPE, nullptr) };
        std::setlocale(LC_CTYPE, o_locale->name().c_str());
        const auto* const p_grapheme_splitter = tetengo_text_graphemeSplitter_create();
        BOOST_SCOPE_EXIT(&previous_locale_name, p_grapheme_splitter)
        {
            tetengo_text_graphemeSplitter_destroy(p_grapheme_splitter);
            std::setlocale(LC_CTYPE, previous_locale_name.c_str());
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_grapheme_splitter);

        {
            const auto grapheme_count =
                tetengo_text_graphemeSplitter_split(p_grapheme_splitter, string_iruka.c_str(), nullptr);
            BOOST_TEST_REQUIRE(grapheme_count == 5U);
            std::vector<tetengo_text_grapheme_t> graphemes(grapheme_count);
            const auto                           grapheme_count_again =
                tetengo_text_graphemeSplitter_split(p_grapheme_splitter, string_iruka.c_str(), std::data(graphemes));
            BOOST_TEST(grapheme_count_again == grapheme_count);
            BOOST_TEST(graphemes[0].offset == 0U);
            BOOST_TEST(graphemes[0].width == 1U);
            BOOST_TEST(graphemes[1].offset == 1U);
            BOOST_TEST(graphemes[1].width == 1U);
            BOOST_TEST(graphemes[2].offset == 2U);
            BOOST_TEST(graphemes[2].width == 1U);
            BOOST_TEST(graphemes[3].offset == 3U);
            BOOST_TEST(graphemes[3].width == 2U);
            BOOST_TEST(graphemes[4].offset == 6U);
            BOOST_TEST(graphemes[4].width == 2U);
        }
        {
            const auto grapheme_count =
                tetengo_text_graphemeSplitter_split(p_grapheme_splitter, string_emoji_cyrillic_greek.c_str(), nullptr);
            BOOST_TEST_REQUIRE(grapheme_count == 4U);
            std::vector<tetengo_text_grapheme_t> graphemes(grapheme_count);
            const auto                           grapheme_count_again = tetengo_text_graphemeSplitter_split(
                p_grapheme_splitter, string_emoji_cyrillic_greek.c_str(), std::data(graphemes));
            BOOST_TEST(grapheme_count_again == grapheme_count);
            BOOST_TEST(graphemes[0].offset == 0U);
            BOOST_TEST(graphemes[0].width == 2U);
            BOOST_TEST(graphemes[1].offset == 3U);
            BOOST_TEST(graphemes[1].width == 2U);
            BOOST_TEST(graphemes[2].offset == 30U);
            BOOST_TEST(graphemes[2].width == 2U);
            BOOST_TEST(graphemes[3].offset == 32U);
            BOOST_TEST(graphemes[3].width == 2U);
        }
    }
    if (auto o_locale = make_locale_ko(); o_locale)
    {
        const std::string previous_locale_name{ std::setlocale(LC_CTYPE, nullptr) };
        std::setlocale(LC_CTYPE, o_locale->name().c_str());
        const auto* const p_grapheme_splitter = tetengo_text_graphemeSplitter_create();
        BOOST_SCOPE_EXIT(&previous_locale_name, p_grapheme_splitter)
        {
            tetengo_text_graphemeSplitter_destroy(p_grapheme_splitter);
            std::setlocale(LC_CTYPE, previous_locale_name.c_str());
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_grapheme_splitter);

        {
            const auto grapheme_count =
                tetengo_text_graphemeSplitter_split(p_grapheme_splitter, string_iruka.c_str(), nullptr);
            BOOST_TEST_REQUIRE(grapheme_count == 5U);
            std::vector<tetengo_text_grapheme_t> graphemes(grapheme_count);
            const auto                           grapheme_count_again =
                tetengo_text_graphemeSplitter_split(p_grapheme_splitter, string_iruka.c_str(), std::data(graphemes));
            BOOST_TEST(grapheme_count_again == grapheme_count);
            BOOST_TEST(graphemes[0].offset == 0U);
            BOOST_TEST(graphemes[0].width == 1U);
            BOOST_TEST(graphemes[1].offset == 1U);
            BOOST_TEST(graphemes[1].width == 1U);
            BOOST_TEST(graphemes[2].offset == 2U);
            BOOST_TEST(graphemes[2].width == 1U);
            BOOST_TEST(graphemes[3].offset == 3U);
            BOOST_TEST(graphemes[3].width == 2U);
            BOOST_TEST(graphemes[4].offset == 6U);
            BOOST_TEST(graphemes[4].width == 2U);
        }
        {
            const auto grapheme_count =
                tetengo_text_graphemeSplitter_split(p_grapheme_splitter, string_emoji_cyrillic_greek.c_str(), nullptr);
            BOOST_TEST_REQUIRE(grapheme_count == 4U);
            std::vector<tetengo_text_grapheme_t> graphemes(grapheme_count);
            const auto                           grapheme_count_again = tetengo_text_graphemeSplitter_split(
                p_grapheme_splitter, string_emoji_cyrillic_greek.c_str(), std::data(graphemes));
            BOOST_TEST(grapheme_count_again == grapheme_count);
            BOOST_TEST(graphemes[0].offset == 0U);
            BOOST_TEST(graphemes[0].width == 2U);
            BOOST_TEST(graphemes[1].offset == 3U);
            BOOST_TEST(graphemes[1].width == 2U);
            BOOST_TEST(graphemes[2].offset == 30U);
            BOOST_TEST(graphemes[2].width == 2U);
            BOOST_TEST(graphemes[3].offset == 32U);
            BOOST_TEST(graphemes[3].width == 2U);
        }
    }
    if (auto o_locale = make_locale_en(); o_locale)
    {
        const std::string previous_locale_name{ std::setlocale(LC_CTYPE, nullptr) };
        std::setlocale(LC_CTYPE, o_locale->name().c_str());
        const auto* const p_grapheme_splitter = tetengo_text_graphemeSplitter_create();
        BOOST_SCOPE_EXIT(&previous_locale_name, p_grapheme_splitter)
        {
            tetengo_text_graphemeSplitter_destroy(p_grapheme_splitter);
            std::setlocale(LC_CTYPE, previous_locale_name.c_str());
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_grapheme_splitter);

        {
            const auto grapheme_count =
                tetengo_text_graphemeSplitter_split(p_grapheme_splitter, string_iruka.c_str(), nullptr);
            BOOST_TEST_REQUIRE(grapheme_count == 5U);
            std::vector<tetengo_text_grapheme_t> graphemes(grapheme_count);
            const auto                           grapheme_count_again =
                tetengo_text_graphemeSplitter_split(p_grapheme_splitter, string_iruka.c_str(), std::data(graphemes));
            BOOST_TEST(grapheme_count_again == grapheme_count);
            BOOST_TEST(graphemes[0].offset == 0U);
            BOOST_TEST(graphemes[0].width == 1U);
            BOOST_TEST(graphemes[1].offset == 1U);
            BOOST_TEST(graphemes[1].width == 1U);
            BOOST_TEST(graphemes[2].offset == 2U);
            BOOST_TEST(graphemes[2].width == 1U);
            BOOST_TEST(graphemes[3].offset == 3U);
            BOOST_TEST(graphemes[3].width == 2U);
            BOOST_TEST(graphemes[4].offset == 6U);
            BOOST_TEST(graphemes[4].width == 2U);
        }
        {
            const auto grapheme_count =
                tetengo_text_graphemeSplitter_split(p_grapheme_splitter, string_emoji_cyrillic_greek.c_str(), nullptr);
            BOOST_TEST_REQUIRE(grapheme_count == 4U);
            std::vector<tetengo_text_grapheme_t> graphemes(grapheme_count);
            const auto                           grapheme_count_again = tetengo_text_graphemeSplitter_split(
                p_grapheme_splitter, string_emoji_cyrillic_greek.c_str(), std::data(graphemes));
            BOOST_TEST(grapheme_count_again == grapheme_count);
            BOOST_TEST(graphemes[0].offset == 0U);
            BOOST_TEST(graphemes[0].width == 1U);
            BOOST_TEST(graphemes[1].offset == 3U);
            BOOST_TEST(graphemes[1].width == 2U);
            BOOST_TEST(graphemes[2].offset == 30U);
            BOOST_TEST(graphemes[2].width == 1U);
            BOOST_TEST(graphemes[3].offset == 32U);
            BOOST_TEST(graphemes[3].width == 1U);
        }
    }
    {
        const auto grapheme_count = tetengo_text_graphemeSplitter_split(nullptr, string_iruka.c_str(), nullptr);
        BOOST_TEST(grapheme_count == 0U);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
