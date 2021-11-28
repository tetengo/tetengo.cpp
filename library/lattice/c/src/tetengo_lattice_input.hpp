/*! \file
    \brief An input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_INPUT_HPP_)
#define TETENGO_LATTICE_INPUT_HPP_

#include <memory>
#include <utility>

#include <tetengo/lattice/input.hpp>


struct tetengo_lattice_input_tag
{
    std::unique_ptr<tetengo::lattice::input> p_cpp_input;

    explicit tetengo_lattice_input_tag(std::unique_ptr<tetengo::lattice::input>&& p_cpp_input) :
    p_cpp_input{ std::move(p_cpp_input) }
    {}
};


#endif
