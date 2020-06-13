/*! \file
    \brief A constraint.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <utility>

#include <tetengo/lattice/constraint.h>
#include <tetengo/lattice/constraint.hpp>


struct tetengo_lattice_constraint_tag
{
    std::unique_ptr<tetengo::lattice::constraint> p_cpp_constraint;

    explicit tetengo_lattice_constraint_tag(std::unique_ptr<tetengo::lattice::constraint>&& p_cpp_constraint) :
    p_cpp_constraint{ std::move(p_cpp_constraint) }
    {}
};


const tetengo_lattice_constraint_t* tetengo_lattice_constraint_createEmpty()
{
    try
    {
        auto p_cpp_constraint = std::make_unique<tetengo::lattice::constraint>();

        auto p_instance = std::make_unique<tetengo_lattice_constraint_t>(std::move(p_cpp_constraint));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_constraint_destroy(const tetengo_lattice_constraint_t* const p_constraint)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_constraint_t> p_instance{ p_constraint };
    }
    catch (...)
    {}
}
