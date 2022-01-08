/*! \file
    \brief A UTF-8 encoding.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <iterator>
#include <string>
#include <string_view>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/text/encoding/utf8.hpp>


namespace
{
    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    constexpr char8_t operator""_u8c(unsigned long long value)
    {
        return static_cast<char8_t>(value);
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
    const std::u8string string1_utf8{
        // clang-format off
        0xE7_u8c, 0x89_u8c, 0xB9_u8c, 0xE6_u8c, 0x80_u8c, 0xA5_u8c,
        0xE3_u8c, 0x83_u8c, 0xA9_u8c, 0xE3_u8c, 0x83_u8c, 0x94_u8c, 0xE3_u8c, 0x83_u8c, 0xBC_u8c, 0xE3_u8c, 0x83_u8c, 0x88_u8c,
        0xCE_u8c, 0xB2_u8c,
        0x32_u8c, 0x35_u8c, 0xE5_u8c, 0x8F_u8c, 0xB7_u8c,
        0xE9_u8c, 0x96_u8c, 0xA2_u8c, 0xE8_u8c, 0xA5_u8c, 0xBF_u8c, 0xE7_u8c, 0xA9_u8c, 0xBA_u8c, 0xE6_u8c, 0xB8_u8c, 0xAF_u8c,
        0xE3_u8c, 0x82_u8c, 0x86_u8c, 0xE3_u8c, 0x81_u8c, 0x8D_u8c,
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
    const std::u8string string2_utf8{
        // clang-format off
        0xEF_u8c, 0xBD_u8c, 0x9E_u8c,
        0x5C_u8c,
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
    const std::u8string string3_utf8{
        // clang-format off
        0xE6_u8c, 0x96_u8c, 0xB0_u8c, 0xE5_u8c, 0xB9_u8c, 0xB9_u8c, 0xE7_u8c, 0xB7_u8c, 0x9A_u8c,
        0xF0_u8c, 0x9F_u8c, 0x9A_u8c, 0x85_u8c,
        0xE3_u8c, 0x81_u8c, 0x8B_u8c, 0xE3_u8c, 0x82_u8c, 0x89_u8c,
        0xE8_u8c, 0xB7_u8c, 0xAF_u8c, 0xE9_u8c, 0x9D_u8c, 0xA2_u8c, 0xE9_u8c, 0x9B_u8c, 0xBB_u8c, 0xE8_u8c, 0xBB_u8c, 0x8A_u8c,
        0xF0_u8c, 0x9F_u8c, 0x9A_u8c, 0x8B_u8c,
        0xE3_u8c, 0x81_u8c, 0xB8_u8c,
        // clang-format on
    };

    // invalid UTF-8 sequence
    const std::string string4_internal{
        // clang-format off
        0xC0_c, 0xAF_c,
        // clang-format on
    };

    // invalid UTF-8 sequence
    const std::u8string string4_utf8{
        // clang-format off
        0xC0_u8c, 0xAF_u8c,
        // clang-format on
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(text)
BOOST_AUTO_TEST_SUITE(encoding)
BOOST_AUTO_TEST_SUITE(utf8)


BOOST_AUTO_TEST_CASE(instance)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const auto& encoding = tetengo::text::encoding::utf8::instance();
}

BOOST_AUTO_TEST_CASE(encode)
{
    BOOST_TEST_PASSPOINT();

    const auto& encoding = tetengo::text::encoding::utf8::instance();
    {
        const auto encoded = encoding.encode(string1_internal);
        BOOST_CHECK(encoded == string1_utf8);
    }
    {
        const auto encoded = encoding.encode(string2_internal);
        BOOST_CHECK(encoded == string2_utf8);
    }
    {
        const auto encoded = encoding.encode(string3_internal);
        BOOST_CHECK(encoded == string3_utf8);
    }
    {
        const auto encoded = encoding.encode(string4_internal);
        BOOST_CHECK(encoded == string4_utf8);
    }
    {
        const auto encoded = encoding.encode("");
        BOOST_TEST(std::empty(encoded));
    }
}

BOOST_AUTO_TEST_CASE(decode)
{
    BOOST_TEST_PASSPOINT();

    const auto& encoding = tetengo::text::encoding::utf8::instance();
    {
        const auto decoded = encoding.decode(string1_utf8);
        BOOST_TEST(decoded == string1_internal);
    }
    {
        const auto decoded = encoding.decode(string2_utf8);
        BOOST_TEST(decoded == string2_internal);
    }
    {
        const auto decoded = encoding.decode(string3_utf8);
        BOOST_TEST(decoded == string3_internal);
    }
    {
        const auto decoded = encoding.decode(string4_utf8);
        BOOST_TEST(decoded == string4_internal);
    }
    {
        const auto decoded = encoding.decode(u8"");
        BOOST_TEST(std::empty(decoded));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
