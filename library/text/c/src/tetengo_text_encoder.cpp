/*! \file
    \brief An encoder.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>

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

namespace
{
    template <typename FromString, typename ToChar>
    void copy_string(const FromString& from, ToChar* const to, std::size_t to_capacity)
    {
        if (!to || to_capacity == 0)
        {
            return;
        }

        const auto length_to_copy = std::min(from.length(), to_capacity);
        std::copy(std::begin(from), std::next(std::begin(from), length_to_copy), to);
        if (from.length() < to_capacity)
        {
            to[from.length()] = 0;
        }
    }

}

size_t tetengo_text_encoder_encode(
    const tetengo_text_encoder_t* const p_encoder,
    const char* const                   string,
    char* const                         encoded_string,
    const size_t                        encoded_string_capacity)
{
    try
    {
        if (!p_encoder)
        {
            throw std::invalid_argument{ "p_encoder is NULL." };
        }
        if (!string)
        {
            throw std::invalid_argument{ "string is NULL." };
        }

        switch (p_encoder->encoding)
        {
        case tetengo_text_encoder_encoding_utf8:
        {
            const auto& encoder =
                dynamic_cast<const tetengo::text::encoder<tetengo::text::encoding::utf8>&>(p_encoder->cpp_encoder);
            const auto encoded = encoder.encode(string);
            copy_string(encoded, encoded_string, encoded_string_capacity);
            return encoded.length();
        }
        case tetengo_text_encoder_encoding_utf16:
        {
            const auto& encoder =
                dynamic_cast<const tetengo::text::encoder<tetengo::text::encoding::utf16>&>(p_encoder->cpp_encoder);
            const auto encoded = encoder.encode(string);
            static_assert(sizeof(char16_t) == sizeof(unsigned short));
            copy_string(encoded, reinterpret_cast<unsigned short*>(encoded_string), encoded_string_capacity);
            return encoded.length();
        }
        case tetengo_text_encoder_encoding_cp932:
        {
            const auto& encoder =
                dynamic_cast<const tetengo::text::encoder<tetengo::text::encoding::cp932>&>(p_encoder->cpp_encoder);
            const auto encoded = encoder.encode(string);
            copy_string(encoded, encoded_string, encoded_string_capacity);
            return encoded.length();
        }
        default:
            assert(false);
            throw std::logic_error{ "Invalid encoding." };
        }
    }
    catch (...)
    {
        return 0;
    }
}

size_t tetengo_text_encoder_decode(
    const tetengo_text_encoder_t* const p_encoder,
    const char* const                   encoded_string,
    char* const                         string,
    const size_t                        string_capacity)
{
    try
    {
        if (!p_encoder)
        {
            throw std::invalid_argument{ "p_encoder is NULL." };
        }
        if (!encoded_string)
        {
            throw std::invalid_argument{ "encoded_string is NULL." };
        }

        switch (p_encoder->encoding)
        {
        case tetengo_text_encoder_encoding_utf8:
        {
            const auto& encoder =
                dynamic_cast<const tetengo::text::encoder<tetengo::text::encoding::utf8>&>(p_encoder->cpp_encoder);
            static_assert(sizeof(char8_t) == sizeof(char));
            const auto decoded = encoder.decode(reinterpret_cast<const char8_t*>(encoded_string));
            copy_string(decoded, string, string_capacity);
            return decoded.length();
        }
        case tetengo_text_encoder_encoding_utf16:
        {
            const auto& encoder =
                dynamic_cast<const tetengo::text::encoder<tetengo::text::encoding::utf16>&>(p_encoder->cpp_encoder);
            static_assert(sizeof(char16_t) == sizeof(unsigned short));
            const auto decoded = encoder.decode(reinterpret_cast<const char16_t*>(encoded_string));
            copy_string(decoded, string, string_capacity);
            return decoded.length();
        }
        case tetengo_text_encoder_encoding_cp932:
        {
            const auto& encoder =
                dynamic_cast<const tetengo::text::encoder<tetengo::text::encoding::cp932>&>(p_encoder->cpp_encoder);
            const auto decoded = encoder.decode(encoded_string);
            copy_string(decoded, string, string_capacity);
            return decoded.length();
        }
        default:
            assert(false);
            throw std::logic_error{ "Invalid encoding." };
        }
    }
    catch (...)
    {
        return 0;
    }
}
