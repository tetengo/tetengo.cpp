/*! \file
    \brief A character width.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_GRAPHEMESPLITTING_CHARACTERWIDTH_HPP)
#define TETENGO_TEXT_GRAPHEMESPLITTING_CHARACTERWIDTH_HPP

#include <cstddef>

#include <boost/core/noncopyable.hpp>


namespace tetengo::text::grapheme_splitting
{
    /*!
        \brief A character width.
    */
    class character_width : private boost::noncopyable
    {
    public:
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

        //! An emoji.
        enum class emoji_type
        {
            emoji, //!< Emoji character.
            normal, //!< Normal charactger.
        };


        // constructors and destructor

        /*!
            \brief Destroys the character width.
        */
        virtual ~character_width();


        // functions

        /*!
            \brief Returns the width.

            \param class_ A class.
            \param emoji  An emoji.

            \return The width of the code point.
        */
        [[nodiscard]] std::size_t width_of(class_type class_, emoji_type emoji) const;


    private:
        // virtual functions

        virtual std::size_t width_of_impl(class_type class_, emoji_type emoji) const = 0;
    };


}


#endif
