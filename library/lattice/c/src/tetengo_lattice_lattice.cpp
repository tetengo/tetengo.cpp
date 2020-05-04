/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <string_view>
#include <utility>

#include <tetengo/lattice/lattice.h>
#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/node.h> // IWYU pragma: keep
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/string_view.h>
#include <tetengo/lattice/vocabulary.h>

#include "tetengo_lattice_vocabulary.hpp" // IWYU pragma: keep


struct tetengo_lattice_lattice_tag
{
    std::unique_ptr<tetengo::lattice::lattice> p_cpp_lattice;

    std::unique_ptr<tetengo_lattice_vocabulary_t> p_vocabulary;

    explicit tetengo_lattice_lattice_tag(
        std::unique_ptr<tetengo::lattice::lattice>&& p_cpp_lattice,
        tetengo_lattice_vocabulary_t* const          p_vocabulary) :
    p_cpp_lattice{ std::move(p_cpp_lattice) },
        p_vocabulary{ p_vocabulary }
    {}
};

tetengo_lattice_lattice_t* tetengo_lattice_lattice_create(tetengo_lattice_vocabulary_t* const p_vocabulary)
{
    try
    {
        if (!p_vocabulary)
        {
            return nullptr;
        }

        auto p_cpp_lattice = std::make_unique<tetengo::lattice::lattice>(std::move(p_vocabulary->p_cpp_vocabulary));

        auto p_instance = std::make_unique<tetengo_lattice_lattice_t>(std::move(p_cpp_lattice), p_vocabulary);
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_lattice_destroy(const tetengo_lattice_lattice_t* const p_lattice)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_lattice_t> p_instance{ p_lattice };
    }
    catch (...)
    {}
}

int tetengo_lattice_lattice_pushBack(tetengo_lattice_lattice_t* const p_lattice, const char* const input)
{
    try
    {
        if (!p_lattice)
        {
            return 0;
        }
        if (!input)
        {
            return 0;
        }

        p_lattice->p_cpp_lattice->push_back(input);

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}

int tetengo_lattice_lattice_settle(tetengo_lattice_lattice_t* const p_lattice, tetengo_lattice_node_t* const p_node)
{
    try
    {
        if (!p_lattice)
        {
            return 0;
        }
        if (!p_node)
        {
            return 0;
        }

        const auto cpp_eos_node = p_lattice->p_cpp_lattice->settle();

        p_node->key.p_head = cpp_eos_node.key().data();
        p_node->key.length = cpp_eos_node.key().length();
        p_node->p_value = &cpp_eos_node.value();
        p_node->preceding = cpp_eos_node.preceding();
        p_node->node_cost = cpp_eos_node.node_cost();
        p_node->path_cost = cpp_eos_node.path_cost();

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}
