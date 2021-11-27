/*! \file
    \brief An input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include "tetengo_lattice_input.hpp"

#include <stddef.h>

#include <tetengo/lattice/input.h>


tetengo_lattice_input_t* tetengo_lattice_input_createStringInput(const char* const /*value*/)
{
    try
    {
        return nullptr;
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_input_destroy(const tetengo_lattice_input_t* const p_input)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_input_t> p_instance{ p_input };
    }
    catch (...)
    {}
}

size_t tetengo_lattice_input_length(const tetengo_lattice_input_t* const /*p_input*/)
{
    try
    {
        return 0;
    }
    catch (...)
    {
        return 0;
    }
}
