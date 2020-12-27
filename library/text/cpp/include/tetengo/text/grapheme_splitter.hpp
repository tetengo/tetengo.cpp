/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_GRAPHEMESPLITTER_HPP)
#define TETENGO_TEXT_GRAPHEMESPLITTER_HPP

#include <cstddef>
#include <locale>
#include <memory>
#include <string_view>
#include <vector>

#include <boost/core/noncopyable.hpp>
#include <boost/operators.hpp>


namespace tetengo::text
{
    /*!
        \brief A grapheme.
    */
    class grapheme : public boost::equality_comparable<grapheme>
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a grapheme.

            \param offset An offset in the UTF-8 string.
            \param width  A width when using a monospace font.
        */
        constexpr grapheme(std::size_t offset, std::size_t width) : m_offset{ offset }, m_width{ width } {}


        // functions

        /*!
            \brief Returns true when one location is equal to another.

            \param one   One location.
            \param another Another location.

            \retval true  When one is equal to another.
            \retval false Otherwise.
        */
        [[nodiscard]] friend constexpr bool operator==(const grapheme& one, const grapheme& another)
        {
            return one.m_offset == another.m_offset && one.m_width == another.m_width;
        }

        /*!
            \brief Returns the offset in the UTF-8 string.

            \return The offset.
        */
        [[nodiscard]] constexpr std::size_t offset() const
        {
            return m_offset;
        }

        /*!
            \brief Returns the width when using a monospace font.

            \return The width.
        */
        [[nodiscard]] constexpr std::size_t width() const
        {
            return m_width;
        }


    private:
        // variables

        std::size_t m_offset;

        std::size_t m_width;
    };


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
            \brief Split a string to graphemes.

            For emoji defined as <a href="http://www.unicode.org/reports/tr11/">"neutral" width</a>
            in the Unicode standard, this function returns a character width of 2 in the East Asian
            (Chinese, Japanese or Korean) locale, or 1 in the other locale.

            \param string_ A string.

            \return Graphemes.

            \throw std::invalid_argument When string_ is not in valid UTF-8.
        */
        [[nodiscard]] std::vector<grapheme> split(const std::string_view& string_) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
