/*! \file
    \brief A CP932 encoding.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <string>
#include <string_view>

#include <boost/core/noncopyable.hpp>

#include <tetengo/platform_dependent/text_encoder.hpp>
#include <tetengo/text/encoding/cp932.hpp>


namespace tetengo::text::encoding
{
    class cp932::impl : private boost::noncopyable
    {
    public:
        // types

        using encoded_string_type = typename cp932::encoded_string_type;


        // static functions

        static const cp932& instance()
        {
            static const cp932 singleton{};
            return singleton;
        }


        // functions

        encoded_string_type encode(const std::string_view& utf8) const
        {
            return tetengo::platform_dependent::text_encoder::instance().encode_to_cp932(utf8);
        }

        std::string decode(const encoded_string_type& string_) const
        {
            return tetengo::platform_dependent::text_encoder::instance().decode_from_cp932(string_);
        }
    };


    const cp932& cp932::instance()
    {
        return impl::instance();
    }

    cp932::~cp932() = default;

    cp932::encoded_string_type cp932::encode(const std::string_view& utf8) const
    {
        return m_p_impl->encode(utf8);
    }

    std::string cp932::decode(const encoded_string_type& string_) const
    {
        return m_p_impl->decode(string_);
    }

    cp932::cp932() : m_p_impl{ std::make_unique<impl>() } {}


}
