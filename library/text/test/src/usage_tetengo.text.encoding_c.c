/*! \file
    \brief The usage of character encoding conversion.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include "usage_tetengo.text.encoding_c.h"

// [encoding]
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include <tetengo/text/encoder.h>


static int equal(const unsigned short* string1, const unsigned short* string2);

void usage_tetengo_text_encoding()
{
    static const char utf8[] = {
        // clang-format off
        // HIGH-SPEED TRAIN WITH BULLET NOSE
        (char)(unsigned char)0xF0, (char)(unsigned char)0x9F, (char)(unsigned char)0x9A, (char)(unsigned char)0x85, // U+1F685
        // POLAR BEAR
        (char)(unsigned char)0xF0, (char)(unsigned char)0x9F, (char)(unsigned char)0x90, (char)(unsigned char)0xBB, // U+1F43B
        (char)(unsigned char)0xE2, (char)(unsigned char)0x80, (char)(unsigned char)0x8D, // U+200D
        (char)(unsigned char)0xE2, (char)(unsigned char)0x9D, (char)(unsigned char)0x84, // U+2744
        (char)(unsigned char)0xEF, (char)(unsigned char)0xB8, (char)(unsigned char)0x8F, // U+FE0F
        // SNOWFLAKE
        (char)(unsigned char)0xE2, (char)(unsigned char)0x9D, (char)(unsigned char)0x84, // U+2744
        0x00,
        // clang-format on
    };
    static const unsigned short utf16[] = {
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
        0x0000,
        // clang-format on
    };

    // Decodes UTF-8 to the internal encoding.
    const tetengo_text_encoder_t* const p_utf8_encoder =
        tetengo_text_encoder_instance(tetengo_text_encoder_encoding_utf8);
    const size_t internal_encoding_length = tetengo_text_encoder_decode(p_utf8_encoder, utf8, NULL, 0);
    char* const  internal_encoding = (char*)malloc((internal_encoding_length + 1) * sizeof(char));
    if (internal_encoding)
    {
        tetengo_text_encoder_decode(p_utf8_encoder, utf8, internal_encoding, internal_encoding_length + 1);
    }

    {
        // Encodes the internal encoding to UTF-16.
        const tetengo_text_encoder_t* const p_utf16_encoder =
            tetengo_text_encoder_instance(tetengo_text_encoder_encoding_utf16);
        const size_t encoded_utf16_length = tetengo_text_encoder_encode(p_utf16_encoder, internal_encoding, NULL, 0);
        unsigned short* const encoded_utf16 =
            (unsigned short*)malloc((encoded_utf16_length + 1) * sizeof(unsigned short));
        if (encoded_utf16)
        {
            tetengo_text_encoder_encode(
                p_utf16_encoder, internal_encoding, (char*)encoded_utf16, encoded_utf16_length + 1);
        }
        assert(equal(encoded_utf16, utf16));
        free(encoded_utf16);
    }

    free((void*)internal_encoding);
}

static int equal(const unsigned short* const string1, const unsigned short* const string2)
{
    size_t i = 0;
    for (i = 0;; ++i)
    {
        const unsigned short c1 = string1[i];
        const unsigned short c2 = string2[i];
        if (c1 != c2)
        {
            return 0;
        }
        if (c1 == 0U)
        {
            return 1;
        }
    }
}
// [encoding]
