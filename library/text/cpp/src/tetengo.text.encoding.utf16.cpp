/*! \file
    \brief A UTF-16 encoding.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <string>
#include <string_view>
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

        using encoded_string_type = typename utf16::encoded_string_type;


        // static functions

        static const utf16& instance()
        {
            static const utf16 singleton{};
            return singleton;
        }


        // functions

        encoded_string_type encode(const std::string_view& utf8) const
        {
            return unicode_encoding::instance().codepoints_to_utf16(
                unicode_encoding::instance().utf8_to_codepoints(utf8).first);
        }

        std::string decode(const encoded_string_type& /*string_*/) const
        {
            return std::string{};
        }
    };


    const utf16& utf16::instance()
    {
        return impl::instance();
    }

    utf16::~utf16() = default;

    utf16::encoded_string_type utf16::encode(const std::string_view& utf8) const
    {
        return m_p_impl->encode(utf8);
    }

    std::string utf16::decode(const encoded_string_type& string_) const
    {
        return m_p_impl->decode(string_);
    }

    utf16::utf16() : m_p_impl{ std::make_unique<impl>() } {}


}
