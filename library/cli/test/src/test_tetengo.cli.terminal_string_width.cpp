/*! \file
    \brief A terminal string width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <locale>
#include <optional>
#include <stdexcept>
#include <string>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/cli/terminal_string_width.hpp>


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

    std::string string_iruka{ 0x4D_c, 0x79_c, 0x20_c, 0xE6_c, 0xB2_c, 0xB3_c, 0xE8_c, 0xB1_c, 0x9A_c };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(cli)
BOOST_AUTO_TEST_SUITE(terminal_string_width)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::cli::terminal_string_width width{};
    }
    if (auto o_locale = make_locale_zh(); o_locale)
    {
        const tetengo::cli::terminal_string_width width{ *o_locale };
    }
    if (auto o_locale = make_locale_ja(); o_locale)
    {
        const tetengo::cli::terminal_string_width width{ *o_locale };
    }
    if (auto o_locale = make_locale_ko(); o_locale)
    {
        const tetengo::cli::terminal_string_width width{ *o_locale };
    }
    if (auto o_locale = make_locale_en(); o_locale)
    {
        const tetengo::cli::terminal_string_width width{ *o_locale };
    }
}

BOOST_AUTO_TEST_CASE(width_of)
{
    BOOST_TEST_PASSPOINT();

    if (auto o_locale = make_locale_zh(); o_locale)
    {
        const tetengo::cli::terminal_string_width tsw{ *o_locale };

        // BOOST_TEST(tsw.width_of(string_iruka) == 7U);
    }
    if (auto o_locale = make_locale_ja(); o_locale)
    {
        const tetengo::cli::terminal_string_width tsw{ *o_locale };

        // BOOST_TEST(tsw.width_of(string_iruka) == 7U);
    }
    if (auto o_locale = make_locale_ko(); o_locale)
    {
        const tetengo::cli::terminal_string_width tsw{ *o_locale };

        // BOOST_TEST(tsw.width_of(string_iruka) == 7U);
    }
    if (auto o_locale = make_locale_en(); o_locale)
    {
        const tetengo::cli::terminal_string_width tsw{ *o_locale };

        // BOOST_TEST(tsw.width_of(string_iruka) == 7U);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
