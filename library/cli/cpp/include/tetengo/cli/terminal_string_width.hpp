/*! \file
    \brief A terminal string width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_CLI_TERMINALSTRINGWIDTH_HPP)
#define TETENGO_CLI_TERMINALSTRINGWIDTH_HPP

#include <locale>
#include <memory>

#include <boost/core/noncopyable.hpp>


namespace tetengo::cli
{
    /*!
        \brief A terminal string width.
    */
    class terminal_string_width : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a terminal string width.

            \param locale_ A locale.
        */
        explicit terminal_string_width(const std::locale& locale_ = std::locale{ "" });

        /*!
            \brief Destroys the terminal string width.
        */
        virtual ~terminal_string_width();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
