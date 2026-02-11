/*! \file
    \brief An input.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_INPUT_HPP_)
#define TETENGO_LATTICE_INPUT_HPP_

#include <memory>
#include <utility>

#include <tetengo/lattice/input.hpp>


struct tetengo_lattice_input_tag
{
private:
    std::unique_ptr<tetengo::lattice::input> p_cpp_input_owned;

    const tetengo::lattice::input* const p_cpp_input_ref;

public:
    explicit tetengo_lattice_input_tag(std::unique_ptr<tetengo::lattice::input>&& p_cpp_input) :
    p_cpp_input_owned{ std::move(p_cpp_input) },
    p_cpp_input_ref{}
    {}

    explicit tetengo_lattice_input_tag(const tetengo::lattice::input& input) :
    p_cpp_input_owned{},
    p_cpp_input_ref{ &input }
    {}

    const tetengo::lattice::input& cpp_input() const
    {
        return p_cpp_input_owned ? *p_cpp_input_owned : *p_cpp_input_ref;
    }

    std::unique_ptr<tetengo::lattice::input>& p_cpp_input()
    {
        return p_cpp_input_owned;
    }
};


#endif
