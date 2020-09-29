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


        // constructors and destructor

        /*!
            \brief Destroys the character width.
        */
        virtual ~character_width();


        // functions

        /*!
            \brief Returns the width.

            \param class_ A class.

            \return The width of the code point.
        */
        [[nodiscard]] std::size_t width_of(class_type class_) const;


    protected:
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

        virtual std::size_t width_of_impl(class_type class_) const = 0;
    };


}


#endif
