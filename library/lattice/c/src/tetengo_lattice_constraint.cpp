/*! \file
    \brief A constraint.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>

#include <tetengo/lattice/constraint.h>
#include <tetengo/lattice/constraint.hpp>
#include <tetengo/lattice/constraintElement.h>
#include <tetengo/lattice/constraint_element.hpp>

#include "tetengo_lattice_constraintElement.hpp" // IWYU pragma: keep


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

const tetengo_lattice_constraint_t* tetengo_lattice_constraint_create(
    tetengo_lattice_constraintElement_t* const* const pp_pattern,
    const size_t                                      pattern_length)
{
    try
    {
        if (!pp_pattern)
        {
            throw std::invalid_argument{ "pp_pattern is NULL." };
        }

        BOOST_SCOPE_EXIT(pp_pattern, pattern_length)
        {
            std::for_each(pp_pattern, pp_pattern + pattern_length, [](auto* const p_constraint_element) {
                tetengo_lattice_constraintElement_destroy(p_constraint_element);
            });
        }
        BOOST_SCOPE_EXIT_END;

        std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> cpp_pattern;
        cpp_pattern.reserve(pattern_length);
        std::for_each(pp_pattern, pp_pattern + pattern_length, [&cpp_pattern](auto* const p_constraint_element) {
            if (!p_constraint_element)
            {
                throw std::invalid_argument{ "pp_pattern has NULL element." };
            }

            cpp_pattern.push_back(std::move(p_constraint_element->p_cpp_constraint_element));
        });

        auto p_cpp_constraint = std::make_unique<tetengo::lattice::constraint>(std::move(cpp_pattern));

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
