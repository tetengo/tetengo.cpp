/*! \file
    \brief A constraint element.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_CONSTRAINTELEMENT_HPP_)
#define TETENGO_LATTICE_CONSTRAINTELEMENT_HPP_

#include <memory>
#include <utility>

#include <tetengo/lattice/constraint_element.hpp>


struct tetengo_lattice_constraintElement_tag
{
    std::unique_ptr<tetengo::lattice::constraint_element> p_cpp_constraint_element;

    explicit tetengo_lattice_constraintElement_tag(
        std::unique_ptr<tetengo::lattice::constraint_element>&& p_cpp_constraint_element) :
    p_cpp_constraint_element{ std::move(p_cpp_constraint_element) }
    {}
};


#endif
