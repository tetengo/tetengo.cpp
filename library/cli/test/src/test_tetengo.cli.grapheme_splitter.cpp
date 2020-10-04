/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <locale>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/cli/grapheme_splitter.hpp>


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
BOOST_AUTO_TEST_SUITE(cli)
BOOST_AUTO_TEST_SUITE(grapheme)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const tetengo::cli::grapheme grapheme_{ 4, 2 };
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::cli::grapheme grapheme1{ 4, 2 };
        const tetengo::cli::grapheme grapheme2{ 4, 2 };

        BOOST_CHECK(grapheme1 == grapheme2);
    }
    {
        const tetengo::cli::grapheme grapheme1{ 4, 2 };
        const tetengo::cli::grapheme grapheme2{ 5, 3 };

        BOOST_CHECK(grapheme1 != grapheme2);
    }
}

BOOST_AUTO_TEST_CASE(offset)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::cli::grapheme grapheme_{ 4, 2 };

    BOOST_TEST(grapheme_.offset() == 4U);
}

BOOST_AUTO_TEST_CASE(width)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::cli::grapheme grapheme_{ 4, 2 };

    BOOST_TEST(grapheme_.width() == 2U);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(grapheme_splitter)
BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::cli::grapheme_splitter splitter{};
    }
    if (auto o_locale = make_locale_zh(); o_locale)
    {
        const tetengo::cli::grapheme_splitter splitter{ *o_locale };
    }
    if (auto o_locale = make_locale_ja(); o_locale)
    {
        const tetengo::cli::grapheme_splitter splitter{ *o_locale };
    }
    if (auto o_locale = make_locale_ko(); o_locale)
    {
        const tetengo::cli::grapheme_splitter splitter{ *o_locale };
    }
    if (auto o_locale = make_locale_en(); o_locale)
    {
        const tetengo::cli::grapheme_splitter splitter{ *o_locale };
    }
}

BOOST_AUTO_TEST_CASE(width_of)
{
    BOOST_TEST_PASSPOINT();

    if (auto o_locale = make_locale_zh(); o_locale)
    {
        const tetengo::cli::grapheme_splitter splitter{ *o_locale };
        {
            const auto graphemes = splitter.split(string_iruka);
            BOOST_TEST_REQUIRE(graphemes.size() == 5U);
            BOOST_TEST((graphemes[0] == tetengo::cli::grapheme{ 0, 1 }));
            BOOST_TEST((graphemes[1] == tetengo::cli::grapheme{ 1, 1 }));
            BOOST_TEST((graphemes[2] == tetengo::cli::grapheme{ 2, 1 }));
            BOOST_TEST((graphemes[3] == tetengo::cli::grapheme{ 3, 2 }));
            BOOST_TEST((graphemes[4] == tetengo::cli::grapheme{ 6, 2 }));
        }
        {
            const auto graphemes = splitter.split(string_emoji_cyrillic_greek);
            BOOST_TEST_REQUIRE(graphemes.size() == 3U);
            BOOST_TEST((graphemes[0] == tetengo::cli::grapheme{ 0, 2 }));
            BOOST_TEST((graphemes[1] == tetengo::cli::grapheme{ 27, 2 }));
            BOOST_TEST((graphemes[2] == tetengo::cli::grapheme{ 29, 2 }));
        }
    }
    if (auto o_locale = make_locale_ja(); o_locale)
    {
        const tetengo::cli::grapheme_splitter splitter{ *o_locale };
        {
            const auto graphemes = splitter.split(string_iruka);
            BOOST_TEST_REQUIRE(graphemes.size() == 5U);
            BOOST_TEST((graphemes[0] == tetengo::cli::grapheme{ 0, 1 }));
            BOOST_TEST((graphemes[1] == tetengo::cli::grapheme{ 1, 1 }));
            BOOST_TEST((graphemes[2] == tetengo::cli::grapheme{ 2, 1 }));
            BOOST_TEST((graphemes[3] == tetengo::cli::grapheme{ 3, 2 }));
            BOOST_TEST((graphemes[4] == tetengo::cli::grapheme{ 6, 2 }));
        }
        {
            const auto graphemes = splitter.split(string_emoji_cyrillic_greek);
            BOOST_TEST_REQUIRE(graphemes.size() == 3U);
            BOOST_TEST((graphemes[0] == tetengo::cli::grapheme{ 0, 2 }));
            BOOST_TEST((graphemes[1] == tetengo::cli::grapheme{ 27, 2 }));
            BOOST_TEST((graphemes[2] == tetengo::cli::grapheme{ 29, 2 }));
        }
    }
    if (auto o_locale = make_locale_ko(); o_locale)
    {
        const tetengo::cli::grapheme_splitter splitter{ *o_locale };
        {
            const auto graphemes = splitter.split(string_iruka);
            BOOST_TEST_REQUIRE(graphemes.size() == 5U);
            BOOST_TEST((graphemes[0] == tetengo::cli::grapheme{ 0, 1 }));
            BOOST_TEST((graphemes[1] == tetengo::cli::grapheme{ 1, 1 }));
            BOOST_TEST((graphemes[2] == tetengo::cli::grapheme{ 2, 1 }));
            BOOST_TEST((graphemes[3] == tetengo::cli::grapheme{ 3, 2 }));
            BOOST_TEST((graphemes[4] == tetengo::cli::grapheme{ 6, 2 }));
        }
        {
            const auto graphemes = splitter.split(string_emoji_cyrillic_greek);
            BOOST_TEST_REQUIRE(graphemes.size() == 3U);
            BOOST_TEST((graphemes[0] == tetengo::cli::grapheme{ 0, 2 }));
            BOOST_TEST((graphemes[1] == tetengo::cli::grapheme{ 27, 2 }));
            BOOST_TEST((graphemes[2] == tetengo::cli::grapheme{ 29, 2 }));
        }
    }
    if (auto o_locale = make_locale_en(); o_locale)
    {
        const tetengo::cli::grapheme_splitter splitter{ *o_locale };
        {
            const auto graphemes = splitter.split(string_iruka);
            BOOST_TEST_REQUIRE(graphemes.size() == 5U);
            BOOST_TEST((graphemes[0] == tetengo::cli::grapheme{ 0, 1 }));
            BOOST_TEST((graphemes[1] == tetengo::cli::grapheme{ 1, 1 }));
            BOOST_TEST((graphemes[2] == tetengo::cli::grapheme{ 2, 1 }));
            BOOST_TEST((graphemes[3] == tetengo::cli::grapheme{ 3, 2 }));
            BOOST_TEST((graphemes[4] == tetengo::cli::grapheme{ 6, 2 }));
        }
        {
            const auto graphemes = splitter.split(string_emoji_cyrillic_greek);
            BOOST_TEST_REQUIRE(graphemes.size() == 3U);
            BOOST_TEST((graphemes[0] == tetengo::cli::grapheme{ 0, 2 }));
            BOOST_TEST((graphemes[1] == tetengo::cli::grapheme{ 27, 1 }));
            BOOST_TEST((graphemes[2] == tetengo::cli::grapheme{ 29, 1 }));
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
