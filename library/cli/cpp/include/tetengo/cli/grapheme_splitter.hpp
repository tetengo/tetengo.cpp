/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_CLI_GRAPHEMESPLITTER_HPP)
#define TETENGO_CLI_GRAPHEMESPLITTER_HPP

#include <cstddef>
#include <locale>
#include <memory>
#include <string_view>

#include <boost/core/noncopyable.hpp>


namespace tetengo::cli
{
    /*!
        \brief A grapheme splitter.
    */
    class grapheme_splitter : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a grapheme splitter.

            \param locale_ A locale.
        */
        explicit grapheme_splitter(const std::locale& locale_ = std::locale{ "" });

        /*!
            \brief Destroys the grapheme splitter.
        */
        virtual ~grapheme_splitter();


        // functions

        /*!
            \brief Returns the string width in the terminal console.

            \param string_ A string.

            \return The string width.

            \throw std::invalid_argument When string_ is not in valid UTF-8.
        */
        [[nodiscard]] std::size_t width_of(const std::string_view& string_) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
