/*! \file
    \brief A constraint.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_CONSTRAINT_HPP_)
#define TETENGO_LATTICE_CONSTRAINT_HPP_

#include <memory>
#include <utility>

#include <tetengo/lattice/constraint.hpp>


struct tetengo_lattice_constraint_tag
{
    std::unique_ptr<tetengo::lattice::constraint> p_cpp_constraint;

    explicit tetengo_lattice_constraint_tag(std::unique_ptr<tetengo::lattice::constraint>&& p_cpp_constraint) :
    p_cpp_constraint{ std::move(p_cpp_constraint) }
    {}
};


#endif
