/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <string_view>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>


const tetengo_lattice_entry_t* tetengo_lattice_entry_bosEos()
{
    try
    {
        static const tetengo_lattice_entry_t singleton{ { tetengo::lattice::entry_view::bos_eos().key().data(),
                                                          tetengo::lattice::entry_view::bos_eos().key().length() },
                                                        tetengo::lattice::entry_view::bos_eos().value(),
                                                        tetengo::lattice::entry_view::bos_eos().cost() };
        return &singleton;
    }
    catch (...)
    {
        return nullptr;
    }
}
