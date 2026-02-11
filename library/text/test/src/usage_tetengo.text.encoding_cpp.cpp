/*! \file
    \brief The usage of character encoding conversion.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include "usage_tetengo.text.encoding_cpp.hpp"

// [encoding]
#include <cassert>
#include <string>

#include <tetengo/text/encoder.hpp>
#include <tetengo/text/encoding/utf16.hpp> // IWYU pragma: keep
#include <tetengo/text/encoding/utf8.hpp> // IWYU pragma: keep


namespace usage_tetengo::text
{
    constexpr char8_t operator""_u8c(unsigned long long value)
    {
        return static_cast<char8_t>(value);
    }

    void encoding()
    {
        static const std::u8string utf8{
            // clang-format off
            // HIGH-SPEED TRAIN WITH BULLET NOSE
            0xF0_u8c, 0x9F_u8c, 0x9A_u8c, 0x85_u8c, // U+1F685
            // POLAR BEAR
            0xF0_u8c, 0x9F_u8c, 0x90_u8c, 0xBB_u8c, // U+1F43B
            0xE2_u8c, 0x80_u8c, 0x8D_u8c, // U+200D
            0xE2_u8c, 0x9D_u8c, 0x84_u8c, // U+2744
            0xEF_u8c, 0xB8_u8c, 0x8F_u8c, // U+FE0F
            // SNOWFLAKE
            0xE2_u8c, 0x9D_u8c, 0x84_u8c, // U+2744
            // clang-format on
        };
        static const std::u16string utf16{
            // clang-format off
            // HIGH-SPEED TRAIN WITH BULLET NOSE
            0xD83D, 0xDE85, // U+1F685
            // POLAR BEAR
            0xD83D, 0xDC3B, // U+1F43B
            0x200D, // U+200D
            0x2744, // U+2744
            0xFE0F, // U+FE0F
            // SNOWFLAKE
            0x2744, // U+2744
            // clang-format on
        };

        // Decodes UTF-8 to the internal encoding.
        using utf8_encoder_type = tetengo::text::encoder<tetengo::text::encoding::utf8>;
        const auto internal_encoding = utf8_encoder_type::instance().decode(utf8);

        // Encodes the internal encoding to UTF-16.
        using utf16_encoder_type = tetengo::text::encoder<tetengo::text::encoding::utf16>;
        const auto encoded_utf16 = utf16_encoder_type::instance().encode(internal_encoding);
        assert(encoded_utf16 == utf16);
    }
}
// [encoding]
