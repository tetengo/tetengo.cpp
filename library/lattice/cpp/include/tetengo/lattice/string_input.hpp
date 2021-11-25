/*! \file
    \brief A string input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_STRINGINPUT_HPP)
#define TETENGO_LATTICE_STRINGINPUT_HPP

#include <cstddef>
#include <memory>
#include <string>

#include <tetengo/lattice/input.hpp>

namespace tetengo::lattice
{
    /*!
        \brief A string input.

        \tparam T A type.
    */
    class string_input : public input
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a string input key.

            \param value A value.
        */
        explicit string_input(std::string value);

        /*!
            \brief Destroys the string input.
        */
        virtual ~string_input();


        // functions

        /*!
            \brief Returns the value.

            \return The value.
        */
        const std::string& value() const;

        /*!
            \brief Returns the value.

            \return The value.
        */
        std::string& value();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual std::size_t length_impl() const override;
    };


}


#endif
