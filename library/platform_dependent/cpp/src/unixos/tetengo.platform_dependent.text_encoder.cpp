/*! \file
    \brief A text encoder.

    For UNIX.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <string>
#include <string_view>

#include <boost/core/noncopyable.hpp>

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
            return std::string{ utf8 };
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
