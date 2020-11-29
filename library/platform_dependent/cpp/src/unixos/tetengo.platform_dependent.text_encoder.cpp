/*! \file
    \brief A text encoder.

    For UNIX.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <errno.h>
#include <iconv.h>
#include <stddef.h>

#include <boost/core/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>

#include <tetengo/platform_dependent/text_encoder.hpp>


namespace tetengo::platform_dependent
{
    class text_encoder::impl : private boost::noncopyable
    {
    public:
        // static functions

        static const text_encoder& instance()
        {
            static const text_encoder singleton{};
            return singleton;
        }


        // functions

        std::string encode_to_cp932(const std::string_view& utf8) const
        {
            const auto descriptor = ::iconv_open("CP932", "UTF-8");
            if (descriptor == reinterpret_cast<::iconv_t>(-1))
            {
                throw std::invalid_argument{ "Cannot create a text encoder." };
            }
            BOOST_SCOPE_EXIT(descriptor)
            {
                ::iconv_close(descriptor);
            }
            BOOST_SCOPE_EXIT_END;

            auto*             p_current_input = const_cast<char*>(std::data(utf8));
            auto              input_length_left = static_cast<::size_t>(utf8.length());
            std::vector<char> output_buffer(1024, 0);
            std::string       encoded{};
            for (;;)
            {
                auto*      p_current_output = std::data(output_buffer);
                auto       output_length_left = static_cast<::size_t>(std::size(output_buffer));
                const auto irreversible =
                    ::iconv(descriptor, &p_current_input, &input_length_left, &p_current_output, &output_length_left);
                encoded.append(std::data(output_buffer), p_current_output);
                if (irreversible != static_cast<::size_t>(-1))
                {
                    break;
                }
                if (errno == EILSEQ)
                {
                    ++p_current_input;
                    --input_length_left;
                    encoded.append("?");
                }
            }

            return encoded;
        }

        std::string decode_from_cp932(const std::string_view& cp932) const
        {
            return std::string{ cp932 };
        }
    };


    const text_encoder& text_encoder::instance()
    {
        return impl::instance();
    }

    text_encoder::~text_encoder() = default;

    std::string text_encoder::encode_to_cp932(const std::string_view& utf8) const
    {
        return m_p_impl->encode_to_cp932(utf8);
    }

    std::string text_encoder::decode_from_cp932(const std::string_view& cp932) const
    {
        return m_p_impl->decode_from_cp932(cp932);
    }

    text_encoder::text_encoder() : m_p_impl{ std::make_unique<impl>() } {}
}
