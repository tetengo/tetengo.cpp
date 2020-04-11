/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <utility>

#include <tetengo/lattice/unordered_map_vocabulary.hpp>
#include <tetengo/lattice/vocabulary.h>
#include <tetengo/lattice/vocabulary.hpp>


struct tetengo_lattice_vocabulary_tag
{
    std::unique_ptr<tetengo::lattice::vocabulary> p_cpp_vocabulary;

    explicit tetengo_lattice_vocabulary_tag(std::unique_ptr<tetengo::lattice::vocabulary>&& p_cpp_vocabulary) :
    p_cpp_vocabulary{ std::move(p_cpp_vocabulary) }
    {}
};

tetengo_lattice_vocabulary* tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
    const char* const* const /*p_keys*/,
    const tetengo_lattice_entry* const /*p_entries*/,
    const size_t /*value_count*/)
{
    return nullptr;
}

void tetengo_lattice_vocabulary_destroy(const tetengo_lattice_vocabulary* const /*p_vocabulary*/) {}

size_t tetengo_lattice_vocabulary_find(
    const tetengo_lattice_vocabulary* const /*p_vocabulary*/,
    const char* const /*key*/,
    tetengo_lattice_entry* const /*p_entries*/)
{
    return 0;
}
