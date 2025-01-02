/*! \file
    \brief An entry.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <memory>
#include <stdexcept>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/input.h>

#include "tetengo_lattice_input.hpp"

namespace tetengo::lattice
{
    class input;
}


const tetengo_lattice_input_t* tetengo_lattice_entry_createKeyOf(tetengo_lattice_entry_keyHandle_t handle)
{
    try
    {
        const auto* const p_cpp_key = reinterpret_cast<const tetengo::lattice::input*>(handle);
        if (!p_cpp_key)
        {
            return nullptr;
        }
        auto p_key = std::make_unique<tetengo_lattice_input_t>(*p_cpp_key);
        return p_key.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_lattice_entry_keyHandle_t tetengo_lattice_entry_toKeyHandle(const tetengo_lattice_input_t* p_content)
{
    try
    {
        if (!p_content)
        {
            throw std::invalid_argument{ "p_content is NULL." };
        }

        return reinterpret_cast<tetengo_lattice_entry_keyHandle_t>(&p_content->cpp_input());
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_entry_destroyKeyHandle(tetengo_lattice_entry_keyHandle_t) {}

const tetengo_lattice_entryView_t* tetengo_lattice_entryView_bosEos()
{
    try
    {
        static const tetengo_lattice_entryView_t singleton{
            reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(tetengo::lattice::entry_view::bos_eos().p_key()),
            reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(tetengo::lattice::entry_view::bos_eos().value()),
            tetengo::lattice::entry_view::bos_eos().cost()
        };
        return &singleton;
    }
    catch (...)
    {
        return nullptr;
    }
}

const tetengo_lattice_input_t* tetengo_lattice_entryView_createKeyOf(tetengo_lattice_entryView_keyHandle_t handle)
{
    try
    {
        const auto* const p_cpp_key = reinterpret_cast<const tetengo::lattice::input*>(handle);
        if (!p_cpp_key)
        {
            return nullptr;
        }
        auto p_key = std::make_unique<tetengo_lattice_input_t>(*p_cpp_key);
        return p_key.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_lattice_entryView_keyHandle_t tetengo_lattice_entryView_toKeyHandle(const tetengo_lattice_input_t* p_content)
{
    try
    {
        if (!p_content)
        {
            throw std::invalid_argument{ "p_content is NULL." };
        }

        return reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(&p_content->cpp_input());
    }
    catch (...)
    {
        return nullptr;
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
