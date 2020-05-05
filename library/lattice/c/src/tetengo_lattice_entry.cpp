/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cassert>
#include <string_view>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>


const tetengo_lattice_entry_t* tetengo_lattice_entry_bosEos()
{
    try
    {
        assert(
            tetengo::lattice::entry_view::bos_eos().value() &&
            !tetengo::lattice::entry_view::bos_eos().value()->has_value());
        static const tetengo_lattice_entry_t singleton{ { tetengo::lattice::entry_view::bos_eos().key().data(),
                                                          tetengo::lattice::entry_view::bos_eos().key().length() },
                                                        nullptr,
                                                        tetengo::lattice::entry_view::bos_eos().cost() };
        return &singleton;
    }
    catch (...)
    {
        return nullptr;
    }
}
