/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <stddef.h>

#include <tetengo/lattice/connection.h>
#include <tetengo/lattice/connection.hpp>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/node.hpp>
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
    const tetengo_lattice_entry_map_element* const      p_entry_map,
    const size_t                                        entry_map_size,
    const tetengo_lattice_connection_map_element* const p_connection_map,
    const size_t                                        connection_map_size)
{
    try
    {
        if (!p_entry_map || entry_map_size == 0)
        {
            return nullptr;
        }
        if (!p_connection_map || connection_map_size == 0)
        {
            return nullptr;
        }

        std::unordered_map<std::string, std::vector<tetengo::lattice::entry>> cpp_entry_map{};
        cpp_entry_map.reserve(entry_map_size);
        for (auto i = static_cast<size_t>(0); i < entry_map_size; ++i)
        {
            const auto& map_element = p_entry_map[i];

            std::string cpp_key{ map_element.key.p_head, map_element.key.length };

            std::vector<tetengo::lattice::entry> cpp_entries{};
            cpp_entries.reserve(map_element.entry_count);
            for (auto j = static_cast<size_t>(0); j < map_element.entry_count; ++j)
            {
                const auto& entry = map_element.p_entries[j];

                cpp_entries.emplace_back(
                    std::string{ entry.key.p_head, entry.key.length },
                    *reinterpret_cast<const std::string*>(entry.p_value),
                    entry.cost);
            }

            cpp_entry_map.insert(std::make_pair(std::move(cpp_key), std::move(cpp_entries)));
        }

        std::unordered_map<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int> cpp_connection_map{};
        cpp_connection_map.reserve(connection_map_size);
        for (auto i = static_cast<size_t>(0); i < connection_map_size; ++i)
        {
            const auto& connection_element = p_connection_map[i];

            auto cpp_key = std::make_pair(
                tetengo::lattice::entry{
                    std::string{ connection_element.p_from->key.p_head, connection_element.p_from->key.length },
                    *reinterpret_cast<const std::string*>(connection_element.p_from->p_value),
                    connection_element.p_from->cost },
                tetengo::lattice::entry{
                    std::string{ connection_element.p_to->key.p_head, connection_element.p_to->key.length },
                    *reinterpret_cast<const std::string*>(connection_element.p_to->p_value),
                    connection_element.p_to->cost });

            cpp_connection_map.insert(std::make_pair(std::move(cpp_key), connection_element.cost));
        }

        auto p_cpp_vocabulary = std::make_unique<tetengo::lattice::unordered_map_vocabulary>(
            std::move(cpp_entry_map), std::move(cpp_connection_map));

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

size_t tetengo_lattice_vocabulary_findEntries(
    const tetengo_lattice_vocabulary* const p_vocabulary,
    const char* const                       key,
    tetengo_lattice_entry* const            p_entries)
{
    try
    {
        const auto found = p_vocabulary->p_cpp_vocabulary->find_entries(key);

        if (p_entries)
        {
            for (auto i = static_cast<std::size_t>(0); i < found.size(); ++i)
            {
                const auto& cpp_entry = found[i];
                auto&       entry = p_entries[i];

                entry.key.p_head = cpp_entry.key().data();
                entry.key.length = cpp_entry.key().length();
                entry.p_value = std::any_cast<std::string>(cpp_entry.value());
                entry.cost = cpp_entry.cost();
            }
        }

        return found.size();
    }
    catch (...)
    {
        return 0;
    }
}

int tetengo_lattice_vocabulary_findConnection(
    const tetengo_lattice_vocabulary* const p_vocabulary,
    const tetengo_lattice_entry* const      p_from,
    const tetengo_lattice_entry* const      p_to,
    tetengo_lattice_connection* const       p_connection)
{
    try
    {
        if (!p_vocabulary)
        {
            return 0;
        }
        if (!p_from)
        {
            return 0;
        }
        if (!p_to)
        {
            return 0;
        }
        if (!p_connection)
        {
            return 0;
        }

        const std::any               cpp_from_value{ *reinterpret_cast<const std::string*>(p_from->p_value) };
        const tetengo::lattice::node cpp_from{ tetengo::lattice::entry_view{
            std::string_view{ p_from->key.p_head, p_from->key.length }, &cpp_from_value, p_from->cost } };
        const std::any               cpp_to_value{ *reinterpret_cast<const std::string*>(p_to->p_value) };
        const tetengo::lattice::node cpp_to{ tetengo::lattice::entry_view{
            std::string_view{ p_to->key.p_head, p_to->key.length }, &cpp_to_value, p_to->cost } };
        const auto                   found = p_vocabulary->p_cpp_vocabulary->find_connection(cpp_from, cpp_to);

        p_connection->cost = found.cost();

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}