/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <utility>

#include <tetengo/lattice/lattice.h>
#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/vocabulary.h>

#include "tetengo_lattice_vocabulary.hpp" // IWYU pragma: keep


struct tetengo_lattice_lattice_tag
{
    std::unique_ptr<tetengo::lattice::lattice> p_cpp_lattice;

    std::unique_ptr<tetengo_lattice_vocabulary_t> p_vocabulary;

    explicit tetengo_lattice_lattice_tag(
        std::unique_ptr<tetengo::lattice::lattice>&& p_cpp_lattice,
        tetengo_lattice_vocabulary_t* const          p_vocabulary) :
    p_cpp_lattice{ std::move(p_cpp_lattice) },
        p_vocabulary{ p_vocabulary }
    {}
};

tetengo_lattice_lattice_t* tetengo_lattice_lattice_create(tetengo_lattice_vocabulary_t* const p_vocabulary)
{
    try
    {
        if (!p_vocabulary)
        {
            return nullptr;
        }

        auto p_cpp_lattice = std::make_unique<tetengo::lattice::lattice>(std::move(p_vocabulary->p_cpp_vocabulary));

        auto p_instance = std::make_unique<tetengo_lattice_lattice_t>(std::move(p_cpp_lattice), p_vocabulary);
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_lattice_destroy(const tetengo_lattice_lattice_t* const p_lattice)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_lattice_t> p_instance{ p_lattice };
    }
    catch (...)
    {}
}

int tetengo_lattice_lattice_pushBack(tetengo_lattice_lattice_t* const p_lattice, const char* const input)
{
    try
    {
        if (!p_lattice)
        {
            return 0;
        }
        if (!input)
        {
            return 0;
        }

        p_lattice->p_cpp_lattice->push_back(input);

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}
