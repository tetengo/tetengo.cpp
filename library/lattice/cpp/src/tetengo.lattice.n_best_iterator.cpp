/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <utility>

#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/n_best_iterator.hpp>
#include <tetengo/lattice/node.hpp>


namespace tetengo::lattice
{
    n_best_iterator::n_best_iterator() : m_p_lattice{}, m_eos_node{ node::bos() } {}

    n_best_iterator::n_best_iterator(const lattice& lattice_, node eos_node) :
    m_p_lattice{ &lattice_ },
        m_eos_node{ std::move(eos_node) }
    {
        m_p_lattice->step_count();
    }


}
