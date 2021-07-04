/*! \file
    \brief A lattice.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_LATTICE_HPP_)
#define TETENGO_LATTICE_LATTICE_HPP_

#include <memory>
#include <utility>

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


#endif
