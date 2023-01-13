/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_VOCABULARY_HPP_)
#define TETENGO_LATTICE_VOCABULARY_HPP_

#include <memory>
#include <utility>

#include <tetengo/lattice/vocabulary.hpp>


struct tetengo_lattice_vocabulary_tag
{
    std::unique_ptr<tetengo::lattice::vocabulary> p_cpp_vocabulary;

    explicit tetengo_lattice_vocabulary_tag(std::unique_ptr<tetengo::lattice::vocabulary>&& p_cpp_vocabulary) :
    p_cpp_vocabulary{ std::move(p_cpp_vocabulary) }
    {}
};


#endif
