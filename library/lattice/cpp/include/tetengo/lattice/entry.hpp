/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_ENTRY_HPP)
#define TETENGO_LATTICE_ENTRY_HPP

#include <string_view>


namespace tetengo::lattice
{
    /*!
        \brief An entry.
    */
    class entry
    {
    public:
        // constructors

        /*!
            \brief Creates an entry.

            \param key     A key.
            \param surface A surface.
            \param cost    A cost.
        */
        entry(std::string_view key, std::string_view surface, int cost);


    private:
        // variables

        std::string_view m_key;

        std::string_view m_surface;

        int m_cost;
    };


}


#endif
