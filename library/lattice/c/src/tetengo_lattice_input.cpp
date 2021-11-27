/*! \file
    \brief An input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include "tetengo_lattice_input.hpp"
#include <memory>
#include <stdexcept>
#include <utility>

#include <stddef.h>

#include <tetengo/lattice/input.h>
#include <tetengo/lattice/string_input.hpp>


tetengo_lattice_input_t* tetengo_lattice_input_createStringInput(const char* const value)
{
    try
    {
        if (!value)
        {
            throw std::invalid_argument{ "value is NULL." };
        }

        auto p_cpp_input = std::make_unique<tetengo::lattice::string_input>(value);

        auto p_instance = std::make_unique<tetengo_lattice_input_t>(std::move(p_cpp_input));
        return p_instance.release();
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
