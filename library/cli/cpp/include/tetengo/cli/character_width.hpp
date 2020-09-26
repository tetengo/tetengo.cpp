/*! \file
    \brief A character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_CLI_CHARACTERWIDTH_HPP)
#define TETENGO_CLI_CHARACTERWIDTH_HPP

#include <cstddef>
#include <memory>

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

            \param previous_code_point A previous code point. Or 0 for no previous character.
            \param code_point          A code point.

            \return The width of the code point.
        */
        [[nodiscard]] std::size_t width_of(char32_t previous_code_point, char32_t code_point) const;


    protected:
        // static functions

        /*!
            \brief Returns the class.

            \param code_point A code point.

            \return The class.
        */
        static int class_of(char32_t code_point);

        /*!
            \brief Returns true when the class value stands for a combining character.

            \param class_ A class.

            \retval true  When the class value stands for a combining character.
            \retval false Otherwise.
        */
        static bool is_combining(int class_);


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

        virtual std::size_t width_of_impl(char32_t previous_code_point, char32_t code_point) const = 0;
    };


}


#endif
