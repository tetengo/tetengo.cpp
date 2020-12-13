/*! \file
    \brief A string encode.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "encode.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include <tetengo/text/encoder.h>


const char* create_encoded_for_print(const char* string)
{
#if _WIN32
    static const tetengo_text_encoder_encoding_t encoding = tetengo_text_encoder_encoding_cp932;
#else
    static const tetengo_text_encoder_encoding_t encoding = tetengo_text_encoder_encoding_utf8;
#endif
    const tetengo_text_encoder_t* const p_encoder = tetengo_text_encoder_instance(encoding);

    const size_t length = tetengo_text_encoder_encode(p_encoder, string, NULL, 0);
    char* const  encoded = malloc((length + 1) * sizeof(char));
    if (!encoded)
    {
        assert(0);
        return NULL;
    }
    tetengo_text_encoder_encode(p_encoder, string, encoded, length + 1);
    encoded[length] = '\0';

    return encoded;
}
