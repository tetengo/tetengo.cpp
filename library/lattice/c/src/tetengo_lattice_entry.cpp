/*! \file
    \brief An entry.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <climits>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <utility>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/stringView.h>


struct tetengo_lattice_entry_tag
{
    tetengo_lattice_stringView_t key;

    const void* p_value;

    int cost;

    tetengo_lattice_entry_tag(tetengo_lattice_stringView_t key, const void* const p_value, const int cost) :
    key{ std::move(key) },
    p_value{ p_value },
    cost{ cost }
    {}
};


const tetengo_lattice_entryView_t* tetengo_lattice_entry_bosEos()
{
    try
    {
        static const tetengo_lattice_entryView_t singleton{ { std::data(tetengo::lattice::entry_view::bos_eos().key()),
                                                              tetengo::lattice::entry_view::bos_eos().key().length() },
                                                            reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(
                                                                tetengo::lattice::entry_view::bos_eos().value()),
                                                            tetengo::lattice::entry_view::bos_eos().cost() };
        return &singleton;
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_lattice_entry_t*
tetengo_lattice_entry_create(const tetengo_lattice_stringView_t* const p_key, const void* const p_value, const int cost)
{
    try
    {
        if (!p_key)
        {
            throw std::invalid_argument{ "p_key is NULL." };
        }

        auto p_instance = std::make_unique<tetengo_lattice_entry_t>(*p_key, p_value, cost);
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_lattice_entry_t* tetengo_lattice_entry_createWithView(const tetengo_lattice_entryView_t* const p_view)
{
    try
    {
        if (!p_view)
        {
            throw std::invalid_argument{ "p_view is NULL." };
        }

        auto p_instance = std::make_unique<tetengo_lattice_entry_t>(
            p_view->key, tetengo_lattice_entryView_valueOf(p_view->value_handle), p_view->cost);
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_entry_destroy(const tetengo_lattice_entry_t* const p_entry)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_entry_t> p_instance{ p_entry };
    }
    catch (...)
    {}
}

const tetengo_lattice_stringView_t* tetengo_lattice_entry_key(const tetengo_lattice_entry_t* const p_entry)
{
    try
    {
        if (!p_entry)
        {
            throw std::invalid_argument{ "p_entry is NULL." };
        }

        return &p_entry->key;
    }
    catch (...)
    {
        return nullptr;
    }
}

const void* tetengo_lattice_entry_value(const tetengo_lattice_entry_t* const p_entry)
{
    try
    {
        if (!p_entry)
        {
            throw std::invalid_argument{ "p_entry is NULL." };
        }

        return p_entry->p_value;
    }
    catch (...)
    {
        return nullptr;
    }
}

int tetengo_lattice_entry_cost(const tetengo_lattice_entry_t* const p_entry)
{
    try
    {
        if (!p_entry)
        {
            throw std::invalid_argument{ "p_entry is NULL." };
        }

        return p_entry->cost;
    }
    catch (...)
    {
        return INT_MAX;
    }
}

const void* tetengo_lattice_entryView_valueOf(const tetengo_lattice_entryView_valueHandle_t handle)
{
    try
    {
        const auto* const p_cpp_value = reinterpret_cast<const std::any*>(handle);
        return p_cpp_value && p_cpp_value->has_value() ? std::any_cast<const void*>(*p_cpp_value) : nullptr;
    }
    catch (...)
    {
        return nullptr;
    }
}
