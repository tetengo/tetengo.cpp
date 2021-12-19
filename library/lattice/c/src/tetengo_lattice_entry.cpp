/*! \file
    \brief An entry.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cstdlib>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/stringView.h>


const tetengo_lattice_stringView_t* tetengo_lattice_entry_createKeyOf(tetengo_lattice_entry_keyHandle_t handle)
{
    try
    {
        const auto* const p_cpp_key = reinterpret_cast<const std::string*>(handle);
        if (!p_cpp_key)
        {
            return nullptr;
        }
        auto* const p_key =
            static_cast<tetengo_lattice_stringView_t*>(std::malloc(sizeof(tetengo_lattice_stringView_t)));
        if (!p_key)
        {
            return nullptr;
        }
        p_key->p_head = std::data(*p_cpp_key);
        p_key->length = p_cpp_key->length();
        return p_key;
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_temp_freeStringView(const tetengo_lattice_stringView_t* p_string_view)
{
    std::free(const_cast<tetengo_lattice_stringView_t*>(p_string_view));
}

tetengo_lattice_entry_keyHandle_t tetengo_lattice_entry_createKeyHandle(const tetengo_lattice_stringView_t* p_content)
{
    try
    {
        if (!p_content)
        {
            throw std::invalid_argument{ "p_content is NULL." };
        }

        auto p_cpp_key = std::make_unique<std::string>(p_content->p_head, p_content->length);
        return reinterpret_cast<tetengo_lattice_entry_keyHandle_t>(p_cpp_key.release());
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_entry_destroyKeyHandle(tetengo_lattice_entry_keyHandle_t handle)
{
    try
    {
        const std::unique_ptr<const std::string> p_instance{ reinterpret_cast<const std::string*>(handle) };
    }
    catch (...)
    {}
}

const tetengo_lattice_entryView_t* tetengo_lattice_entryView_bosEos()
{
    try
    {
        static const tetengo_lattice_entryView_t singleton{
            reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(&tetengo::lattice::entry_view::bos_eos().key()),
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

const tetengo_lattice_stringView_t* tetengo_lattice_entryView_createKeyOf(tetengo_lattice_entryView_keyHandle_t handle)
{
    try
    {
        const auto* const p_cpp_key = reinterpret_cast<const std::string_view*>(handle);
        if (!p_cpp_key)
        {
            return nullptr;
        }
        auto* const p_key =
            static_cast<tetengo_lattice_stringView_t*>(std::malloc(sizeof(tetengo_lattice_stringView_t)));
        if (!p_key)
        {
            return nullptr;
        }
        p_key->p_head = std::data(*p_cpp_key);
        p_key->length = p_cpp_key->length();
        return p_key;
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
