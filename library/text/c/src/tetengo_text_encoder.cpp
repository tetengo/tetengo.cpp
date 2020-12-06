/*! \file
    \brief An encoder.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <tetengo/text/encoder.h>


struct tetengo_text_encoder_tag
{};


const tetengo_text_encoder_t* tetengo_text_encoder_instance(const tetengo_text_encoder_encoding_t /*encoding*/)
{
    return nullptr;
}
