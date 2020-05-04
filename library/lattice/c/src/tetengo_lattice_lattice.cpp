/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <tetengo/lattice/lattice.h>
#include <tetengo/lattice/vocabulary.h>

#include "tetengo_lattice_vocabulary.hpp" // IWYU pragma: keep


struct tetengo_lattice_lattice_tag
{
    std::unique_ptr<tetengo_lattice_vocabulary_t> p_vocabulary;

    explicit tetengo_lattice_lattice_tag(tetengo_lattice_vocabulary_t* p_vocabulary) : p_vocabulary{ p_vocabulary } {}
};

tetengo_lattice_lattice_t* tetengo_lattice_lattice_create(tetengo_lattice_vocabulary_t* p_vocabulary)
{
    try
    {
        if (!p_vocabulary)
        {
            return nullptr;
        }

        auto p_instance = std::make_unique<tetengo_lattice_lattice_t>(p_vocabulary);
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_lattice_destroy(const tetengo_lattice_lattice_t* p_lattice)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_lattice_t> p_instance{ p_lattice };
    }
    catch (...)
    {}
}
