/*! \file
    \brief A UTF-16 encoding.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <iterator>
#include <string>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/text/encoding/utf16.hpp>


namespace
{
    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    // tokkyu rapi:t beta 25-go kansai-kuko yuki (rapi:t limited express No. 25 bound for Kansai Airport)
    const std::string string1_internal{
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

    // wave-dash and back-slash
    const std::string string2_internal{
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

    // shinkansen (emoji) kara romendensha (emoji) e (From Shinkansen to tram)
    const std::string string3_internal{
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

    // invalid UTF-8 sequence
    const std::string string4_internal{
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

    // invalid UTF-16 sequence
    const std::string string5_internal{
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
BOOST_AUTO_TEST_SUITE(encoding)
BOOST_AUTO_TEST_SUITE(utf16)


BOOST_AUTO_TEST_CASE(instance)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const auto& encoding = tetengo::text::encoding::utf16::instance();
}

BOOST_AUTO_TEST_CASE(encode)
{
    BOOST_TEST_PASSPOINT();

    const auto& encoding = tetengo::text::encoding::utf16::instance();
    {
        const auto encoded = encoding.encode(string1_internal);
        BOOST_CHECK(encoded == string1_utf16);
    }
    {
        const auto encoded = encoding.encode(string2_internal);
        BOOST_CHECK(encoded == string2_utf16);
    }
    {
        const auto encoded = encoding.encode(string3_internal);
        BOOST_CHECK(encoded == string3_utf16);
    }
    {
        const auto encoded = encoding.encode(string4_internal);
        BOOST_CHECK(encoded == string4_utf16);
    }
    {
        const auto encoded = encoding.encode("");
        BOOST_CHECK(std::empty(encoded));
    }
}

BOOST_AUTO_TEST_CASE(decode)
{
    BOOST_TEST_PASSPOINT();

    const auto& encoding = tetengo::text::encoding::utf16::instance();
    {
        const auto decoded = encoding.decode(string1_utf16);
        BOOST_TEST(decoded == string1_internal);
    }
    {
        const auto decoded = encoding.decode(string2_utf16);
        BOOST_TEST(decoded == string2_internal);
    }
    {
        const auto decoded = encoding.decode(string3_utf16);
        BOOST_TEST(decoded == string3_internal);
    }
    {
        const auto decoded = encoding.decode(string5_utf16);
        BOOST_TEST(decoded == string5_internal);
    }
    {
        const auto decoded = encoding.decode(u"");
        BOOST_TEST(std::empty(decoded));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
