/*! \file
    \brief A string encode.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#include "encode.h"

#include <assert.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#include <tetengo/text/encoder.h>


static tetengo_text_encoder_encoding_t select_encoding()
{
    const char* const locale = setlocale(LC_CTYPE, NULL);
    if (locale && strcmp(locale, "Japanese_Japan.932") == 0)
    {
        return tetengo_text_encoder_encoding_cp932;
    }
    else
    {
        return tetengo_text_encoder_encoding_utf8;
    }
}

const char* create_encoded_for_print(const char* const string)
{
    const tetengo_text_encoder_t* const p_encoder = tetengo_text_encoder_instance(select_encoding());

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
