/*! \file
    \brief A UTF-16 encoding.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <string>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/text/encoding/utf16.hpp>

#include "tetengo.text.encoding.unicode_encoding.hpp"


namespace tetengo::text::encoding
{
    class utf16::impl : private boost::noncopyable
    {
    public:
        // types

        using string_type = typename utf16::string_type;

        using string_view_type = typename utf16::string_view_type;

        using encoded_string_type = typename utf16::encoded_string_type;

        using encoded_string_view_type = typename utf16::encoded_string_view_type;


        // static functions

        static const utf16& instance()
        {
            static const utf16 singleton{};
            return singleton;
        }


        // functions

        encoded_string_type encode(const string_view_type& utf8) const
        {
            return unicode_encoding::instance().code_points_to_utf16(
                unicode_encoding::instance().utf8_to_code_points(utf8).first);
        }

        string_type decode(const encoded_string_view_type& string_) const
        {
            return unicode_encoding::instance().code_points_to_utf8(
                unicode_encoding::instance().utf16_to_code_points(string_).first);
        }
    };


    const utf16& utf16::instance()
    {
        return impl::instance();
    }

    utf16::~utf16() = default;

    typename utf16::encoded_string_type utf16::encode(const string_view_type& utf8) const
    {
        return m_p_impl->encode(utf8);
    }

    typename utf16::string_type utf16::decode(const encoded_string_view_type& string_) const
    {
        return m_p_impl->decode(string_);
    }

    utf16::utf16() : m_p_impl{ std::make_unique<impl>() } {}


}
