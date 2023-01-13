/*! \file
    \brief A Unicode encoding.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_ENCODING_UNICODEENCODING_HPP_)
#define TETENGO_TEXT_ENCODING_UNICODEENCODING_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>


namespace tetengo::text::encoding
{
    class unicode_encoding : private boost::noncopyable
    {
    public:
        // static functions

        static const unicode_encoding& instance();


        // constructors and destructor

        ~unicode_encoding();


        // functions

        std::pair<std::u32string, std::vector<std::size_t>> utf8_to_code_points(const std::string_view& utf8) const;

        std::string code_points_to_utf8(const std::u32string_view& code_points) const;

        std::pair<std::u32string, std::vector<std::size_t>>
        utf16_to_code_points(const std::u16string_view& utf16) const;

        std::u16string code_points_to_utf16(const std::u32string_view& code_points) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        unicode_encoding();
    };
}


#endif
