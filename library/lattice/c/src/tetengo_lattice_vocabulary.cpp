/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <typeinfo>
#include <utility>
#include <vector>

#include <stddef.h>

#include <tetengo/lattice/connection.h>
#include <tetengo/lattice/connection.hpp>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/stringView.h>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>
#include <tetengo/lattice/vocabulary.h>
#include <tetengo/lattice/vocabulary.hpp>

#include "tetengo_lattice_vocabulary.hpp"


tetengo_lattice_vocabulary_t* tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
    const tetengo_lattice_keyEntriesPair_t* const            p_entries,
    const size_t                                             entry_count,
    const tetengo_lattice_entriesConnectionCostPair_t* const p_connections,
    const size_t                                             connection_count,
    size_t (*p_entry_hash)(const tetengo_lattice_entryView_t*),
    int (*p_entry_equal_to)(const tetengo_lattice_entryView_t*, const tetengo_lattice_entryView_t*))
{
    try
    {
        if (!p_entries && entry_count > 0)
        {
            throw std::invalid_argument{ "p_entries is NULL." };
        }
        if (!p_connections && connection_count > 0)
        {
            throw std::invalid_argument{ "p_connections is NULL." };
        }

        std::vector<std::pair<std::string, std::vector<tetengo::lattice::entry>>> cpp_entries{};
        cpp_entries.reserve(entry_count);
        for (auto i = static_cast<size_t>(0); i < entry_count; ++i)
        {
            const auto& map_element = p_entries[i];

            std::string cpp_key{ map_element.key.p_head, map_element.key.length };

            std::vector<tetengo::lattice::entry> cpp_entry_values{};
            cpp_entry_values.reserve(map_element.entry_count);
            for (auto j = static_cast<size_t>(0); j < map_element.entry_count; ++j)
            {
                const auto& entry = map_element.p_entries[j];

                std::any cpp_entry_value{ entry.p_value };
                cpp_entry_values.emplace_back(
                    std::string{ entry.key.p_head, entry.key.length }, std::move(cpp_entry_value), entry.cost);
            }

            cpp_entries.emplace_back(std::move(cpp_key), std::move(cpp_entry_values));
        }

        std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>> cpp_connections{};
        cpp_connections.reserve(connection_count);
        for (auto i = static_cast<size_t>(0); i < connection_count; ++i)
        {
            const auto& connection_element = p_connections[i];

            std::any cpp_from_value{ connection_element.p_from->p_value };
            std::any cpp_to_value{ connection_element.p_to->p_value };
            auto     cpp_key = std::make_pair(
                tetengo::lattice::entry{
                    std::string{ connection_element.p_from->key.p_head, connection_element.p_from->key.length },
                    std::move(cpp_from_value),
                    connection_element.p_from->cost },
                tetengo::lattice::entry{
                    std::string{ connection_element.p_to->key.p_head, connection_element.p_to->key.length },
                    std::move(cpp_to_value),
                    connection_element.p_to->cost });

            cpp_connections.emplace_back(std::move(cpp_key), connection_element.cost);
        }

        auto p_cpp_vocabulary = std::make_unique<tetengo::lattice::unordered_map_vocabulary>(
            std::move(cpp_entries),
            std::move(cpp_connections),
            [p_entry_hash](const tetengo::lattice::entry_view& cpp_entry) {
                tetengo_lattice_entryView_t entry{};
                entry.key.p_head = std::data(cpp_entry.key());
                entry.key.length = cpp_entry.key().length();
                entry.value_handle = reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(cpp_entry.value());
                entry.cost = cpp_entry.cost();
                return p_entry_hash(&entry);
            },
            [p_entry_equal_to](
                const tetengo::lattice::entry_view& cpp_entry1, const tetengo::lattice::entry_view& cpp_entry2) {
                tetengo_lattice_entryView_t entry1{};
                entry1.key.p_head = std::data(cpp_entry1.key());
                entry1.key.length = cpp_entry1.key().length();
                entry1.value_handle = reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(cpp_entry1.value());
                entry1.cost = cpp_entry1.cost();

                tetengo_lattice_entryView_t entry2{};
                entry2.key.p_head = std::data(cpp_entry2.key());
                entry2.key.length = cpp_entry2.key().length();
                entry2.value_handle = reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(cpp_entry2.value());
                entry2.cost = cpp_entry2.cost();

                return p_entry_equal_to(&entry1, &entry2);
            });

        auto p_instance = std::make_unique<tetengo_lattice_vocabulary_t>(std::move(p_cpp_vocabulary));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_vocabulary_destroy(const tetengo_lattice_vocabulary_t* const p_vocabulary)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_vocabulary_t> p_instance{ p_vocabulary };
    }
    catch (...)
    {}
}

size_t tetengo_lattice_vocabulary_findEntries(
    const tetengo_lattice_vocabulary_t* const p_vocabulary,
    const char* const                         key,
    tetengo_lattice_entryView_t* const        p_entries)
{
    try
    {
        if (!p_vocabulary)
        {
            throw std::invalid_argument{ "p_vocabulary is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }

        const auto found = p_vocabulary->p_cpp_vocabulary->find_entries(key);

        if (p_entries)
        {
            for (auto i = static_cast<std::size_t>(0); i < std::size(found); ++i)
            {
                const auto& cpp_entry = found[i];
                auto&       entry = p_entries[i];

                entry.key.p_head = std::data(cpp_entry.key());
                entry.key.length = cpp_entry.key().length();
                assert(cpp_entry.value()->type() == typeid(const void*));
                entry.value_handle = reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(cpp_entry.value());
                entry.cost = cpp_entry.cost();
            }
        }

        return std::size(found);
    }
    catch (...)
    {
        return 0;
    }
}

int tetengo_lattice_vocabulary_findConnection(
    const tetengo_lattice_vocabulary_t* const p_vocabulary,
    const tetengo_lattice_node_t* const       p_from,
    const tetengo_lattice_entryView_t* const  p_to,
    tetengo_lattice_connection_t* const       p_connection)
{
    try
    {
        if (!p_vocabulary)
        {
            throw std::invalid_argument{ "p_vocabulary is NULL." };
        }
        if (!p_from)
        {
            throw std::invalid_argument{ "p_from is NULL." };
        }
        if (!p_to)
        {
            throw std::invalid_argument{ "p_to is NULL." };
        }
        if (!p_connection)
        {
            throw std::invalid_argument{ "p_connection is NULL." };
        }

        const std::vector<int>             cpp_preceding_edge_costs{};
        const tetengo::lattice::node       cpp_from{ std::string_view{ p_from->key.p_head, p_from->key.length },
                                               reinterpret_cast<const std::any*>(p_from->value_handle),
                                               p_from->preceding_step,
                                               &cpp_preceding_edge_costs,
                                               p_from->best_preceding_node,
                                               p_from->node_cost,
                                               p_from->path_cost };
        const tetengo::lattice::entry_view cpp_to{ std::string_view{ p_to->key.p_head, p_to->key.length },
                                                   reinterpret_cast<const std::any*>(p_to->value_handle),
                                                   p_to->cost };
        const auto                         found = p_vocabulary->p_cpp_vocabulary->find_connection(cpp_from, cpp_to);

        p_connection->cost = found.cost();

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}
