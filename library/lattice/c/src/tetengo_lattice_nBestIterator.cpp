/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <utility>

#include <tetengo/lattice/nBestIterator.h>
#include <tetengo/lattice/n_best_iterator.hpp>


struct tetengo_lattice_nBestIterator_tag
{
    std::unique_ptr<tetengo::lattice::n_best_iterator> p_cpp_iterator;

    explicit tetengo_lattice_nBestIterator_tag(std::unique_ptr<tetengo::lattice::n_best_iterator>&& p_cpp_iterator) :
    p_cpp_iterator{ std::move(p_cpp_iterator) }
    {}
};

tetengo_lattice_nBestIterator_t* tetengo_lattice_nBestIterator_createEnd()
{
    try
    {
        auto p_cpp_iterator = std::make_unique<tetengo::lattice::n_best_iterator>();

        auto p_instance = std::make_unique<tetengo_lattice_nBestIterator_t>(std::move(p_cpp_iterator));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_nBestIterator_destroy(const tetengo_lattice_nBestIterator_t* const p_iterator)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_nBestIterator_t> p_instance{ p_iterator };
    }
    catch (...)
    {}
}
