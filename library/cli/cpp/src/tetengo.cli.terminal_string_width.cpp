/*! \file
    \brief A terminal string width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <locale>
#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/cli/terminal_string_width.hpp>


namespace tetengo::cli
{
    class terminal_string_width::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(const std::locale& locale_) : m_locale{ locale_ } {}


    private:
        // variables

        const std::locale m_locale;
    };


    terminal_string_width::terminal_string_width(const std::locale& locale_ /*= std::locale{ "" }*/) :
    m_p_impl{ std::make_unique<impl>(locale_) }
    {}

    terminal_string_width::~terminal_string_width() = default;
}
