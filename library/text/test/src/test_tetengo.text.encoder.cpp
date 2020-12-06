/*! \file
    \brief An encoder.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/text/encoder.hpp>
#include <tetengo/text/encoding/cp932.hpp> // IWYU pragma: keep
#include <tetengo/text/encoding/utf16.hpp> // IWYU pragma: keep
#include <tetengo/text/encoding/utf8.hpp> // IWYU pragma: keep


namespace
{
    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    // tokkyu rapi:t beta 25-go kansai-kuko yuki (rapi:t limited express No. 25 bound for Kansai Airport)
    const std::string string1_utf8{
        // clang-format off
        0xE7_c, 0x89_c, 0xB9_c, 0xE6_c, 0x80_c, 0xA5_c,
        0xE3_c, 0x83_c, 0xA9_c, 0xE3_c, 0x83_c, 0x94_c, 0xE3_c, 0x83_c, 0xBC_c, 0xE3_c, 0x83_c, 0x88_c,
        0xCE_c, 0xB2_c,
        0x32_c, 0x35_c, 0xE5_c, 0x8F_c, 0xB7_c,
        0xE9_c, 0x96_c, 0xA2_c, 0xE8_c, 0xA5_c, 0xBF_c, 0xE7_c, 0xA9_c, 0xBA_c, 0xE6_c, 0xB8_c, 0xAF_c,
        0xE3_c, 0x82_c, 0x86_c, 0xE3_c, 0x81_c, 0x8D_c,
        // clang-format on
    };

    // tokkyu rapi:t beta 25-go kansai-kuko yuki (rapi:t limited express No. 25 bound for Kansai Airport)
    const std::u16string string1_utf16{
        // clang-format off
        0x7279, 0x6025,
        0x30E9, 0x30D4, 0x30FC, 0x30C8,
        0x03B2,
        0x0032, 0x0035, 0x53F7,
        0x95A2, 0x897F, 0x7A7A, 0x6E2F,
        0x3086, 0x304D,
        // clang-format on
    };

    // tokkyu rapi:t beta 25-go kansai-kuko yuki (rapi:t limited express No. 25 bound for Kansai Airport)
    const std::string string1_cp932{
        // clang-format off
        0x93_c, 0xC1_c, 0x8B_c, 0x7D_c,
        0x83_c, 0x89_c, 0x83_c, 0x73_c, 0x81_c, 0x5B_c, 0x83_c, 0x67_c,
        0x83_c, 0xC0_c,
        0x32_c, 0x35_c, 0x8D_c, 0x86_c,
        0x8A_c, 0xD6_c, 0x90_c, 0xBC_c, 0x8B_c, 0xF3_c, 0x8D_c, 0x60_c,
        0x82_c, 0xE4_c, 0x82_c, 0xAB_c,
        // clang-format on
    };

    // wave-dash and back-slash
    const std::string string2_utf8{
        // clang-format off
        0xEF_c, 0xBD_c, 0x9E_c,
        0x5C_c,
        // clang-format on
    };

    // wave-dash and back-slash
    const std::u16string string2_utf16{
        // clang-format off
        0xFF5E,
        0x005C,
        // clang-format on
    };

    // wave-dash and back-slash
    const std::string string2_cp932{
        // clang-format off
        0x81_c, 0x60_c,
        0x5C_c,
        // clang-format on
    };

    // shinkansen (emoji) kara romendensha (emoji) e (From Shinkansen to tram)
    const std::string string3_utf8{
        // clang-format off
        0xE6_c, 0x96_c, 0xB0_c, 0xE5_c, 0xB9_c, 0xB9_c, 0xE7_c, 0xB7_c, 0x9A_c,
        0xF0_c, 0x9F_c, 0x9A_c, 0x85_c,
        0xE3_c, 0x81_c, 0x8B_c, 0xE3_c, 0x82_c, 0x89_c,
        0xE8_c, 0xB7_c, 0xAF_c, 0xE9_c, 0x9D_c, 0xA2_c, 0xE9_c, 0x9B_c, 0xBB_c, 0xE8_c, 0xBB_c, 0x8A_c,
        0xF0_c, 0x9F_c, 0x9A_c, 0x8B_c,
        0xE3_c, 0x81_c, 0xB8_c,
        // clang-format on
    };

    // shinkansen (emoji) kara romendensha (emoji) e (From Shinkansen to tram)
    const std::u16string string3_utf16{
        // clang-format off
        0x65B0, 0x5E79, 0x7DDA,
        0xD83D, 0xDE85,
        0x304B, 0x3089,
        0x8DEF, 0x9762, 0x96FB, 0x8ECA,
        0xD83D, 0xDE8B,
        0x3078,
        // clang-format on
    };

    // shinkansen (emoji) kara romendensha (emoji) e (From Shinkansen to tram)
    const std::string pattern3_cp932{
        // clang-format off
        0x90_c, 0x56_c, 0x8A_c, 0xB2_c, 0x90_c, 0xFC_c,
        '\\', 0x3F_c, '+',
        0x82_c, 0xA9_c, 0x82_c, 0xE7_c,
        0x98_c, 0x48_c, 0x96_c, 0xCA_c, 0x93_c, 0x64_c, 0x8E_c, 0xD4_c,
        '\\', 0x3F_c, '+',
        0x82_c, 0xD6_c,
        // clang-format on
    };

    // invalid UTF-8 sequence
    const std::string string4_utf8{
        // clang-format off
        0xC0_c, 0xAF_c,
        // clang-format on
    };

    // invalid UTF-8 sequence
    const std::u16string string4_utf16{
        // clang-format off
        0x003F, 0x003F,
        // clang-format on
    };

    // invalid UTF-8 sequence
    const std::string pattern4_cp932{
        // clang-format off
        '\\', 0x3F_c, '+',
        // clang-format on
    };

    // invalid UTF-16 sequence
    const std::string string5_utf8{
        // clang-format off
        0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0xE7_c, 0x89_c, 0xB9_c, 0x003F,
        // clang-format on
    };

    // invalid UTF-16 sequence
    const std::u16string string5_utf16{
        // clang-format off
        0xD800, 0xD800, 0xDC00, 0xD800, 0xD800, 0x7279, 0xD800,
        // clang-format on
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(text)
BOOST_AUTO_TEST_SUITE(encoder)


BOOST_AUTO_TEST_CASE(instance)
{
    BOOST_TEST_PASSPOINT();

    {
        [[maybe_unused]] const auto& encoder = tetengo::text::encoder<tetengo::text::encoding::utf8>::instance();
    }
    {
        [[maybe_unused]] const auto& encoder = tetengo::text::encoder<tetengo::text::encoding::utf16>::instance();
    }
    {
        [[maybe_unused]] const auto& encoder = tetengo::text::encoder<tetengo::text::encoding::cp932>::instance();
    }
}

BOOST_AUTO_TEST_CASE(encode)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto& encoder = tetengo::text::encoder<tetengo::text::encoding::utf8>::instance();
        {
            const auto encoded = encoder.encode(string1_utf8);
            BOOST_TEST(encoded == string1_utf8);
        }
        {
            const auto encoded = encoder.encode(string2_utf8);
            BOOST_TEST(encoded == string2_utf8);
        }
        {
            const auto encoded = encoder.encode(string3_utf8);
            BOOST_TEST(encoded == string3_utf8);
        }
        {
            const auto encoded = encoder.encode(string4_utf8);
            BOOST_TEST(encoded == string4_utf8);
        }
    }
    {
        const auto& encoder = tetengo::text::encoder<tetengo::text::encoding::utf16>::instance();
        {
            const auto encoded = encoder.encode(string1_utf8);
            BOOST_CHECK(encoded == string1_utf16);
        }
        {
            const auto encoded = encoder.encode(string2_utf8);
            BOOST_CHECK(encoded == string2_utf16);
        }
        {
            const auto encoded = encoder.encode(string3_utf8);
            BOOST_CHECK(encoded == string3_utf16);
        }
        {
            const auto encoded = encoder.encode(string4_utf8);
            BOOST_CHECK(encoded == string4_utf16);
        }
    }
    {
        const auto& encoder = tetengo::text::encoder<tetengo::text::encoding::cp932>::instance();
        {
            const auto encoded = encoder.encode(string1_utf8);
            BOOST_TEST(encoded == string1_cp932);
        }
        {
            const auto encoded = encoder.encode(string2_utf8);
            BOOST_TEST(encoded == string2_cp932);
        }
        {
            const auto       encoded = encoder.encode(string3_utf8);
            const std::regex regex_{ pattern3_cp932 };
            BOOST_TEST(std::regex_match(std::string{ encoded }, regex_));
        }
        {
            const auto       encoded = encoder.encode(string4_utf8);
            const std::regex regex_{ pattern4_cp932 };
            BOOST_TEST(std::regex_match(std::string{ encoded }, regex_));
        }
    }
}

BOOST_AUTO_TEST_CASE(decode)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto& encoder = tetengo::text::encoder<tetengo::text::encoding::utf8>::instance();
        {
            const auto decoded = encoder.decode(string1_utf8);
            BOOST_TEST(decoded == string1_utf8);
        }
        {
            const auto decoded = encoder.decode(string2_utf8);
            BOOST_TEST(decoded == string2_utf8);
        }
        {
            const auto decoded = encoder.decode(string3_utf8);
            BOOST_TEST(decoded == string3_utf8);
        }
        {
            const auto decoded = encoder.decode(string4_utf8);
            BOOST_TEST(decoded == string4_utf8);
        }
    }
    {
        const auto& encoder = tetengo::text::encoder<tetengo::text::encoding::utf16>::instance();
        {
            const auto decoded = encoder.decode(string1_utf16);
            BOOST_TEST(decoded == string1_utf8);
        }
        {
            const auto decoded = encoder.decode(string2_utf16);
            BOOST_TEST(decoded == string2_utf8);
        }
        {
            const auto decoded = encoder.decode(string3_utf16);
            BOOST_TEST(decoded == string3_utf8);
        }
        {
            const auto decoded = encoder.decode(string5_utf16);
            BOOST_TEST(decoded == string5_utf8);
        }
    }
    {
        const auto& encoder = tetengo::text::encoder<tetengo::text::encoding::cp932>::instance();
        {
            const auto decoded = encoder.decode(string1_cp932);
            BOOST_TEST(decoded == string1_utf8);
        }
        {
            const auto decoded = encoder.decode(string2_cp932);
            BOOST_TEST(decoded == string2_utf8);
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
