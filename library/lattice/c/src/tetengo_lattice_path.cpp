/*! \file
    \brief A path.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <any>
#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/path.h>
#include <tetengo/lattice/path.hpp>
#include <tetengo/lattice/stringView.h>


struct tetengo_lattice_path_tag
{
    std::vector<std::vector<int>> p_cpp_preceding_edge_cost_lists;

    std::unique_ptr<tetengo::lattice::path> p_cpp_path;

    tetengo_lattice_path_tag(
        std::vector<std::vector<int>>&&           p_cpp_preceding_edge_cost_lists,
        std::unique_ptr<tetengo::lattice::path>&& p_cpp_path) :
    p_cpp_preceding_edge_cost_lists{ std::move(p_cpp_preceding_edge_cost_lists) },
    p_cpp_path{ std::move(p_cpp_path) }
    {}
};


tetengo_lattice_path_t* tetengo_lattice_path_createEmpty()
{
    try
    {
        auto p_cpp_path = std::make_unique<tetengo::lattice::path>();

        auto p_instance =
            std::make_unique<tetengo_lattice_path_t>(std::vector<std::vector<int>>{}, std::move(p_cpp_path));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_lattice_path_t*
tetengo_lattice_path_create(const tetengo_lattice_node_t* const p_nodes, const size_t node_count, const int cost)
{
    try
    {
        if (!p_nodes && node_count > 0)
        {
            throw std::invalid_argument{ "p_nodes is NULL." };
        }

        std::vector<std::vector<int>> cpp_preceding_edge_cost_lists{};
        cpp_preceding_edge_cost_lists.reserve(node_count);
        for (auto i = static_cast<std::size_t>(0); i < node_count; ++i)
        {
            const auto&      node = p_nodes[i];
            std::vector<int> cpp_preceding_edge_cost_list{};
            cpp_preceding_edge_cost_list.reserve(node.preceding_edge_cost_count);
            std::copy(
                node.p_preceding_edge_costs,
                std::next(node.p_preceding_edge_costs, node.preceding_edge_cost_count),
                std::back_inserter(cpp_preceding_edge_cost_list));
            cpp_preceding_edge_cost_lists.push_back(std::move(cpp_preceding_edge_cost_list));
        }

        std::vector<tetengo::lattice::node> cpp_nodes{};
        cpp_nodes.reserve(node_count);
        for (auto i = static_cast<std::size_t>(0); i < node_count; ++i)
        {
            const auto&       node = p_nodes[i];
            const auto* const p_cpp_node_key = tetengo_lattice_entryView_createKeyOf(node.key_handle);
            BOOST_SCOPE_EXIT(p_cpp_node_key)
            {
                tetengo_lattice_temp_freeStringView(p_cpp_node_key);
            }
            BOOST_SCOPE_EXIT_END;
            const auto& cpp_preceding_edge_cost_list = cpp_preceding_edge_cost_lists[i];
            cpp_nodes.emplace_back(
                p_cpp_node_key ? std::string_view{ p_cpp_node_key->p_head, p_cpp_node_key->length } :
                                 std::string_view{},
                reinterpret_cast<const std::any*>(node.value_handle),
                node.preceding_step,
                &cpp_preceding_edge_cost_list,
                node.best_preceding_node,
                node.node_cost,
                node.path_cost);
        }

        auto p_cpp_path = std::make_unique<tetengo::lattice::path>(std::move(cpp_nodes), cost);

        auto p_instance =
            std::make_unique<tetengo_lattice_path_t>(std::move(cpp_preceding_edge_cost_lists), std::move(p_cpp_path));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_path_destroy(const tetengo_lattice_path_t* const p_path)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_path_t> p_instance{ p_path };
    }
    catch (...)
    {}
}

int tetengo_lattice_path_empty(const tetengo_lattice_path_t* const p_path)
{
    try
    {
        if (!p_path)
        {
            throw std::invalid_argument{ "p_path is NULL." };
        }

        return std::empty(*p_path->p_cpp_path);
    }
    catch (...)
    {
        return 0;
    }
}

size_t tetengo_lattice_path_pNodes(const tetengo_lattice_path_t* const p_path, tetengo_lattice_node_t* const p_nodes)
{
    try
    {
        if (!p_path)
        {
            throw std::invalid_argument{ "p_path is NULL." };
        }

        if (p_nodes)
        {
            std::transform(
                std::begin(p_path->p_cpp_path->nodes()),
                std::end(p_path->p_cpp_path->nodes()),
                p_nodes,
                [](const auto& cpp_node) {
                    tetengo_lattice_node_t c_node{};
                    c_node.key_handle = reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(&cpp_node.key());
                    c_node.value_handle = reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&cpp_node.value());
                    c_node.preceding_step = cpp_node.preceding_step();
                    c_node.p_preceding_edge_costs = std::data(cpp_node.preceding_edge_costs());
                    c_node.preceding_edge_cost_count = std::size(cpp_node.preceding_edge_costs());
                    c_node.best_preceding_node = cpp_node.best_preceding_node();
                    c_node.node_cost = cpp_node.node_cost();
                    c_node.path_cost = cpp_node.path_cost();
                    return c_node;
                });
        }

        return std::size(p_path->p_cpp_path->nodes());
    }
    catch (...)
    {
        return 0;
    }
}

int tetengo_lattice_path_cost(const tetengo_lattice_path_t* const p_path)
{
    try
    {
        if (!p_path)
        {
            throw std::invalid_argument{ "p_path is NULL." };
        }

        return p_path->p_cpp_path->cost();
    }
    catch (...)
    {
        return 0;
    }
}
