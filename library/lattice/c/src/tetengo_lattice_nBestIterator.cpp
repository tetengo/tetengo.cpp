/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <memory>
#include <string_view>
#include <utility>

#include <tetengo/lattice/nBestIterator.h>
#include <tetengo/lattice/n_best_iterator.hpp>
#include <tetengo/lattice/node.h> // IWYU pragma: keep
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/stringView.h>
#include <tetengo/lattice/vocabulary.h>

#include "tetengo_lattice_lattice.hpp"
#include "tetengo_lattice_vocabulary.hpp" // IWYU pragma: keep


struct tetengo_lattice_nBestIterator_tag
{
    std::unique_ptr<tetengo::lattice::n_best_iterator> p_cpp_iterator;

    explicit tetengo_lattice_nBestIterator_tag(std::unique_ptr<tetengo::lattice::n_best_iterator>&& p_cpp_iterator) :
    p_cpp_iterator{ std::move(p_cpp_iterator) }
    {}
};

tetengo_lattice_nBestIterator_t* tetengo_lattice_nBestIterator_createEnd()
{
    try
    {
        auto p_cpp_iterator = std::make_unique<tetengo::lattice::n_best_iterator>();

        auto p_instance = std::make_unique<tetengo_lattice_nBestIterator_t>(std::move(p_cpp_iterator));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_lattice_nBestIterator_t* tetengo_lattice_nBestIterator_createBegin(
    const tetengo_lattice_lattice_t* const p_lattice,
    const tetengo_lattice_node_t* const    p_eos_node)
{
    try
    {
        if (!p_lattice)
        {
            return nullptr;
        }
        if (!p_eos_node)
        {
            return nullptr;
        }

        tetengo::lattice::node cpp_eos_node{ std::string_view{ p_eos_node->key.p_head, p_eos_node->key.length },
                                             reinterpret_cast<const std::any*>(p_eos_node->value_handle),
                                             p_eos_node->preceding_step,
                                             p_eos_node->best_preceding_node,
                                             p_eos_node->node_cost,
                                             p_eos_node->path_cost };
        auto                   p_cpp_iterator =
            std::make_unique<tetengo::lattice::n_best_iterator>(*p_lattice->p_cpp_lattice, std::move(cpp_eos_node));

        auto p_instance = std::make_unique<tetengo_lattice_nBestIterator_t>(std::move(p_cpp_iterator));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_nBestIterator_destroy(const tetengo_lattice_nBestIterator_t* const p_iterator)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_nBestIterator_t> p_instance{ p_iterator };
    }
    catch (...)
    {}
}
