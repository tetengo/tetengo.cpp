/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <stddef.h>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/string_view.h>
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
    const tetengo_lattice_entry_map_element* const p_map,
    const size_t                                   map_size)
{
    try
    {
        if (!p_map)
        {
            return nullptr;
        }

        std::unordered_map<std::string, std::vector<tetengo::lattice::entry>> cpp_map{};
        cpp_map.reserve(map_size);
        for (auto i = static_cast<size_t>(0); i < map_size; ++i)
        {
            const auto& map_element = p_map[i];

            std::string cpp_key{ map_element.key.p_head, map_element.key.length };

            std::vector<tetengo::lattice::entry> cpp_entries{};
            cpp_entries.reserve(map_element.entry_count);
            for (auto j = static_cast<size_t>(0); j < map_element.entry_count; ++j)
            {
                const auto& entry = map_element.p_entries[j];

                cpp_entries.emplace_back(
                    std::string{ entry.key.p_head, entry.key.length },
                    std::string{ entry.surface.p_head, entry.surface.length },
                    entry.cost);
            }

            cpp_map.insert(std::make_pair(std::move(cpp_key), std::move(cpp_entries)));
        }

        auto p_cpp_vocabulary = std::make_unique<tetengo::lattice::unordered_map_vocabulary>(std::move(cpp_map));

        auto p_instance = std::make_unique<tetengo_lattice_vocabulary>(std::move(p_cpp_vocabulary));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_vocabulary_destroy(const tetengo_lattice_vocabulary* const p_vocabulary)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_vocabulary> p_instance{ p_vocabulary };
    }
    catch (...)
    {}
}

size_t tetengo_lattice_vocabulary_find(
    const tetengo_lattice_vocabulary* const /*p_vocabulary*/,
    const char* const /*key*/,
    tetengo_lattice_entry* const /*p_entries*/)
{
    return 0;
}
