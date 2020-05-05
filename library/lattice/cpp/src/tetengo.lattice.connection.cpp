/*! \file
    \brief A connection.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <tetengo/lattice/connection.hpp>


namespace tetengo::lattice
{
    connection::connection(int cost) : m_cost{ cost } {}

    int connection::cost() const
    {
        return m_cost;
    }


}
