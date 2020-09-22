/*! \file
    \brief An east Asian character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_CLI_EASTASIANCHARACTERWIDTH_HPP)
#define TETENGO_CLI_EASTASIANCHARACTERWIDTH_HPP

#include <cstddef>
#include <memory>

#include <tetengo/cli/character_width.hpp>


namespace tetengo::cli
{
    /*!
        \brief An east Asian character width.
    */
    class east_asian_character_width : public character_width
    {
    public:
        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        [[nodiscard]] static const east_asian_character_width& instance();


        // constructors and destructor

        /*!
            \brief Destroys the character width.
        */
        virtual ~east_asian_character_width();


    private:
        // tyeps

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        east_asian_character_width();


        // virtual functions

        virtual std::size_t width_of_impl(char32_t previous_code_point, char32_t code_point) const;
    };


}


#endif
