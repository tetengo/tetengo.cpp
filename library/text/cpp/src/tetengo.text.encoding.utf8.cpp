/*! \file
    \brief A UTF-8 encoding.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/text/encoding/utf8.hpp>


namespace tetengo::text::encoding
{
    class utf8::impl : private boost::noncopyable
    {
    public:
        // types

        using string_type = typename utf8::string_type;

        using string_view_type = typename utf8::string_view_type;

        using encoded_string_type = typename utf8::encoded_string_type;

        using encoded_string_view_type = typename utf8::encoded_string_view_type;


        // static functions

        static const utf8& instance()
        {
            static const utf8 singleton{};
            return singleton;
        }


        // functions

        encoded_string_type encode(const string_view_type& utf8) const
        {
            return utf8;
        }

        string_type decode(const encoded_string_view_type& string_) const
        {
            return string_;
        }
    };


    const utf8& utf8::instance()
    {
        return impl::instance();
    }

    utf8::~utf8() = default;

    typename utf8::encoded_string_type utf8::encode(const string_view_type& utf8) const
    {
        return m_p_impl->encode(utf8);
    }

    typename utf8::string_type utf8::decode(const encoded_string_view_type& string_) const
    {
        return m_p_impl->decode(string_);
    }

    utf8::utf8() : m_p_impl{ std::make_unique<impl>() } {}


}
