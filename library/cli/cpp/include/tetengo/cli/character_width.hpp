/*! \file
    \brief A character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_CLI_CHARACTERWIDTH_HPP)
#define TETENGO_CLI_CHARACTERWIDTH_HPP

#include <cstddef>
#include <memory>
#include <utility>

#include <boost/core/noncopyable.hpp>


namespace tetengo::cli
{
    /*!
        \brief A character width.
    */
    class character_width : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Destroys the character width.
        */
        virtual ~character_width();


        // functions

        /*!
            \brief Returns the width.

            \param code_point A code point.

            \return The width of the code point.
        */
        [[nodiscard]] std::size_t width_of(char32_t code_point) const;


    protected:
        // types

        //! A class.
        enum class class_type
        {
            ambiguous, //!< Ambiguous.
            fullwidth, //!< Fullwidth.
            halfwidth, //!< Halfwidth.
            narrow, //!< Narrow.
            wide, //!< Wide.
            neutral, //!< Neutral.
        };

        //! A grapheme.
        enum class grapheme_type
        {
            cr, //!< CR
            lf, //!< LF
            control, //!< Control
            extend, //!< Extend
            zwj, //!< ZWJ
            regional, //!< Regional
            prepend, //!< Prepend
            spacing_mark, //!< SpacingMark
            l, //!< L
            v, //!< V
            t, //!< T
            lv, //!< LV
            lvt, //!< LVT
            other, //!< Other
        };


        // static functions

        /*!
            \brief Returns the property.

            \param code_point A code point.

            \return The property.
        */
        [[nodiscard]] static std::pair<class_type, grapheme_type> property_of(char32_t code_point);


        // constructors

        /*!
            \brief Creates a character width.
        */
        character_width();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual std::size_t width_of_impl(char32_t code_point) const = 0;
    };


}


#endif
