/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>

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
    std::unique_ptr<std::pair<tetengo::lattice::n_best_iterator, tetengo::lattice::n_best_iterator>>
        p_cpp_iterator_pair;

    std::unique_ptr<std::vector<int>> p_cpp_eos_preceding_edge_costs;

    tetengo_lattice_nBestIterator_tag(
        std::unique_ptr<std::pair<tetengo::lattice::n_best_iterator, tetengo::lattice::n_best_iterator>>&&
                                            p_cpp_iterator_pair,
        std::unique_ptr<std::vector<int>>&& p_cpp_eos_preceding_edge_costs) :
    p_cpp_iterator_pair{ std::move(p_cpp_iterator_pair) },
        p_cpp_eos_preceding_edge_costs{ std::move(p_cpp_eos_preceding_edge_costs) }
    {}
};

tetengo_lattice_nBestIterator_t* tetengo_lattice_nBestIterator_create(
    const tetengo_lattice_lattice_t* const p_lattice,
    const tetengo_lattice_node_t* const    p_eos_node)
{
    try
    {
        if (!p_lattice)
        {
            throw std::invalid_argument{ "p_lattice is NULL." };
        }
        if (!p_eos_node)
        {
            throw std::invalid_argument{ "p_eos_node is NULL." };
        }

        auto p_cpp_preceding_edge_costs = std::make_unique<std::vector<int>>(
            p_eos_node->p_preceding_edge_costs,
            p_eos_node->p_preceding_edge_costs + p_eos_node->preceding_edge_cost_count);
        tetengo::lattice::node cpp_eos_node{ std::string_view{ p_eos_node->key.p_head, p_eos_node->key.length },
                                             reinterpret_cast<const std::any*>(p_eos_node->value_handle),
                                             p_eos_node->preceding_step,
                                             p_cpp_preceding_edge_costs.get(),
                                             p_eos_node->best_preceding_node,
                                             p_eos_node->node_cost,
                                             p_eos_node->path_cost };

        auto p_instance = std::make_unique<tetengo_lattice_nBestIterator_t>(
            std::make_unique<std::pair<tetengo::lattice::n_best_iterator, tetengo::lattice::n_best_iterator>>(
                tetengo::lattice::n_best_iterator{ *p_lattice->p_cpp_lattice, std::move(cpp_eos_node) },
                tetengo::lattice::n_best_iterator{}),
            std::move(p_cpp_preceding_edge_costs));
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

size_t tetengo_lattice_nBestIterator_get(
    const tetengo_lattice_nBestIterator_t* const p_iterator,
    tetengo_lattice_node_t* const                p_path)
{
    try
    {
        if (!p_iterator)
        {
            throw std::invalid_argument{ "p_iterator is NULL." };
        }

        const auto& cpp_path = *p_iterator->p_cpp_iterator_pair->first;

        if (p_path)
        {
            for (auto i = static_cast<std::size_t>(0); i < cpp_path.size(); ++i)
            {
                p_path[i].key.p_head = cpp_path[i].key().data();
                p_path[i].key.length = cpp_path[i].key().length();
                p_path[i].value_handle = reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&cpp_path[i].value());
                p_path[i].preceding_step = cpp_path[i].preceding_step();
                p_path[i].p_preceding_edge_costs = cpp_path[i].preceding_edge_costs().data();
                p_path[i].preceding_edge_cost_count = cpp_path[i].preceding_edge_costs().size();
                p_path[i].best_preceding_node = cpp_path[i].best_preceding_node();
                p_path[i].node_cost = cpp_path[i].node_cost();
                p_path[i].path_cost = cpp_path[i].path_cost();
            }
        }

        return cpp_path.size();
    }
    catch (...)
    {
        return 0;
    }
}

int tetengo_lattice_nBestIterator_hasNext(const tetengo_lattice_nBestIterator_t* p_iterator)
{
    try
    {
        if (!p_iterator)
        {
            throw std::invalid_argument{ "p_iterator is NULL." };
        }

        return p_iterator->p_cpp_iterator_pair->first != p_iterator->p_cpp_iterator_pair->second ? 1 : 0;
    }
    catch (...)
    {
        return 0;
    }
}

void tetengo_lattice_nBestIterator_next(tetengo_lattice_nBestIterator_t* p_iterator)
{
    try
    {
        if (!p_iterator)
        {
            throw std::invalid_argument{ "p_iterator is NULL." };
        }

        ++p_iterator->p_cpp_iterator_pair->first;
    }
    catch (...)
    {}
}
