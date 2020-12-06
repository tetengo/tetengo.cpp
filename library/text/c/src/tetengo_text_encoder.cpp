/*! \file
    \brief An encoder.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <stdexcept>

#include <tetengo/text/encoder.h>
#include <tetengo/text/encoder.hpp>
#include <tetengo/text/encoding/cp932.hpp> // IWYU pragma: keep
#include <tetengo/text/encoding/utf16.hpp> // IWYU pragma: keep
#include <tetengo/text/encoding/utf8.hpp> // IWYU pragma: keep


struct tetengo_text_encoder_tag
{
    const tetengo::text::encoder_base& cpp_encoder;

    tetengo_text_encoder_encoding_t encoding;

    tetengo_text_encoder_tag(
        const tetengo::text::encoder_base&    cpp_encoder,
        const tetengo_text_encoder_encoding_t encoding) :
    cpp_encoder{ cpp_encoder },
        encoding{ encoding }
    {}
};


namespace
{
    template <typename CppEncoding, tetengo_text_encoder_encoding_t Encoding>
    const tetengo_text_encoder_t& encoder()
    {
        static const tetengo_text_encoder_t singleton{ tetengo::text::encoder<CppEncoding>::instance(), Encoding };
        return singleton;
    }

}

const tetengo_text_encoder_t* tetengo_text_encoder_instance(const tetengo_text_encoder_encoding_t encoding)
{
    try
    {
        switch (encoding)
        {
        case tetengo_text_encoder_encoding_utf8:
            return &encoder<tetengo::text::encoding::utf8, tetengo_text_encoder_encoding_utf8>();
        case tetengo_text_encoder_encoding_utf16:
            return &encoder<tetengo::text::encoding::utf16, tetengo_text_encoder_encoding_utf16>();
        case tetengo_text_encoder_encoding_cp932:
            return &encoder<tetengo::text::encoding::cp932, tetengo_text_encoder_encoding_cp932>();
        default:
            throw std::invalid_argument{ "Invalid encoding." };
        }
    }
    catch (...)
    {
        return nullptr;
    }
}
