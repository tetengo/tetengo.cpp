/*! \file
    \brief A constraint element.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#include <tetengo/lattice/constraint_element.hpp>


namespace tetengo::lattice
{
    class node;


    constraint_element::~constraint_element() = default;

    int constraint_element::matches(const node& node_) const
    {
        return matches_impl(node_);
    }


}
