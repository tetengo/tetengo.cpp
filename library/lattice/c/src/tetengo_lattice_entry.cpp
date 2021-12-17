/*! \file
    \brief An entry.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <iterator>
#include <string>
#include <string_view>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/stringView.h>


const tetengo_lattice_entryView_t* tetengo_lattice_entryView_bosEos()
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

tetengo_lattice_stringView_t tetengo_lattice_entry_keyOf(tetengo_lattice_entry_keyHandle_t handle)
{
    try
    {
        const auto* const p_cpp_key = reinterpret_cast<const std::string*>(handle);
        return p_cpp_key ? tetengo_lattice_stringView_t{ p_cpp_key->c_str(), p_cpp_key->length() } :
                           tetengo_lattice_stringView_t{ nullptr, 0 };
    }
    catch (...)
    {
        return tetengo_lattice_stringView_t{ nullptr, 0 };
    }
}

tetengo_lattice_stringView_t tetengo_lattice_entryView_keyOf(tetengo_lattice_entryView_keyHandle_t handle)
{
    try
    {
        const auto* const p_cpp_key = reinterpret_cast<const std::string_view*>(handle);
        return p_cpp_key ? tetengo_lattice_stringView_t{ std::data(*p_cpp_key), p_cpp_key->length() } :
                           tetengo_lattice_stringView_t{ nullptr, 0 };
    }
    catch (...)
    {
        return tetengo_lattice_stringView_t{ nullptr, 0 };
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
