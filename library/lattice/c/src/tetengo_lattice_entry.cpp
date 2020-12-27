/*! \file
    \brief An entry.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <iterator>
#include <string_view>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>


const tetengo_lattice_entryView_t* tetengo_lattice_entry_bosEos()
{
    try
    {
        static const tetengo_lattice_entryView_t singleton{ { std::data(tetengo::lattice::entry_view::bos_eos().key()),
                                                              tetengo::lattice::entry_view::bos_eos().key().length() },
                                                            reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(
                                                                tetengo::lattice::entry_view::bos_eos().value()),
                                                            tetengo::lattice::entry_view::bos_eos().cost() };
        return &singleton;
    }
    catch (...)
    {
        return nullptr;
    }
}

const void* tetengo_lattice_entry_valueOf(const tetengo_lattice_entry_valueHandle_t handle)
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
