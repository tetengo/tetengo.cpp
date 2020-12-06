/*! \file
    \brief A CP932 encoding.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/platform_dependent/text_encoder.hpp>
#include <tetengo/text/encoding/cp932.hpp>


namespace tetengo::text::encoding
{
    class cp932::impl : private boost::noncopyable
    {
    public:
        // types

        using string_type = typename cp932::string_type;

        using string_view_type = typename cp932::string_view_type;

        using encoded_string_type = typename cp932::encoded_string_type;

        using encoded_string_view_type = typename cp932::encoded_string_view_type;


        // static functions

        static const cp932& instance()
        {
            static const cp932 singleton{};
            return singleton;
        }


        // functions

        encoded_string_type encode(const string_view_type& utf8) const
        {
            return tetengo::platform_dependent::text_encoder::instance().encode_to_cp932(utf8);
        }

        string_type decode(const encoded_string_view_type& string_) const
        {
            return tetengo::platform_dependent::text_encoder::instance().decode_from_cp932(string_);
        }
    };


    const cp932& cp932::instance()
    {
        return impl::instance();
    }

    cp932::~cp932() = default;

    typename cp932::encoded_string_type cp932::encode(const string_view_type& utf8) const
    {
        return m_p_impl->encode(utf8);
    }

    typename cp932::string_type cp932::decode(const encoded_string_view_type& string_) const
    {
        return m_p_impl->decode(string_);
    }

    cp932::cp932() : m_p_impl{ std::make_unique<impl>() } {}


}
