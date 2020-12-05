/*! \file
    \brief A Unicode encoding.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_ENCODING_UNICODEENCODING_HPP_)
#define TETENGO_TEXT_ENCODING_UNICODEENCODING_HPP_

#include <cstddef>
#include <memory>
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

        std::vector<std::pair<char32_t, std::size_t>> utf8_to_codepoints(const std::string_view& string) const;


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
