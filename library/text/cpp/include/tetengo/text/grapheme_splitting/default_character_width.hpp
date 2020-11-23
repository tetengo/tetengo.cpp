/*! \file
    \brief A default character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_DEFAULTCHARACTERWIDTH_HPP)
#define TETENGO_TEXT_DEFAULTCHARACTERWIDTH_HPP

#include <cstddef>
#include <memory>

#include <tetengo/text/grapheme_splitting/character_width.hpp>


namespace tetengo::text
{
    /*!
        \brief A default character width.
    */
    class default_character_width : public character_width
    {
    public:
        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        [[nodiscard]] static const default_character_width& instance();


        // constructors and destructor

        /*!
            \brief Destroys the character width.
        */
        virtual ~default_character_width();


    private:
        // tyeps

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        default_character_width();


        // virtual functions

        virtual std::size_t width_of_impl(class_type class_, emoji_type emoji) const;
    };


}


#endif
