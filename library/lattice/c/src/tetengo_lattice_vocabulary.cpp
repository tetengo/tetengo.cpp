/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <any>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>

#include <tetengo/lattice/connection.h>
#include <tetengo/lattice/connection.hpp>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/input.h>
#include <tetengo/lattice/input.hpp>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/stringView.h>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>
#include <tetengo/lattice/vocabulary.h>
#include <tetengo/lattice/vocabulary.hpp>

#include "tetengo_lattice_input.hpp"
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

                const auto* const p_cpp_entry_key = tetengo_lattice_entry_createKeyOf(entry.key_handle);
                BOOST_SCOPE_EXIT(p_cpp_entry_key)
                {
                    tetengo_lattice_input_destroy(p_cpp_entry_key);
                }
                BOOST_SCOPE_EXIT_END;
                std::any cpp_entry_value{ entry.p_value };
                cpp_entry_values.emplace_back(
                    p_cpp_entry_key ? p_cpp_entry_key->cpp_input().clone() : nullptr,
                    std::move(cpp_entry_value),
                    entry.cost);
            }

            cpp_entries.emplace_back(std::move(cpp_key), std::move(cpp_entry_values));
        }

        std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>> cpp_connections{};
        cpp_connections.reserve(connection_count);
        for (auto i = static_cast<size_t>(0); i < connection_count; ++i)
        {
            const auto& connection_element = p_connections[i];

            const auto* const p_cpp_from_key = tetengo_lattice_entry_createKeyOf(connection_element.p_from->key_handle);
            BOOST_SCOPE_EXIT(p_cpp_from_key)
            {
                tetengo_lattice_input_destroy(p_cpp_from_key);
            }
            BOOST_SCOPE_EXIT_END;
            std::any          cpp_from_value{ connection_element.p_from->p_value };
            const auto* const p_cpp_to_key = tetengo_lattice_entry_createKeyOf(connection_element.p_to->key_handle);
            BOOST_SCOPE_EXIT(p_cpp_to_key)
            {
                tetengo_lattice_input_destroy(p_cpp_to_key);
            }
            BOOST_SCOPE_EXIT_END;
            std::any cpp_to_value{ connection_element.p_to->p_value };
            auto     cpp_key = std::make_pair(
                tetengo::lattice::entry{ p_cpp_from_key ? p_cpp_from_key->cpp_input().clone() : nullptr,
                                         std::move(cpp_from_value),
                                         connection_element.p_from->cost },
                tetengo::lattice::entry{ p_cpp_to_key ? p_cpp_to_key->cpp_input().clone() : nullptr,
                                         std::move(cpp_to_value),
                                         connection_element.p_to->cost });

            cpp_connections.emplace_back(std::move(cpp_key), connection_element.cost);
        }

        auto p_cpp_vocabulary = std::make_unique<tetengo::lattice::unordered_map_vocabulary>(
            std::move(cpp_entries),
            std::move(cpp_connections),
            [p_entry_hash](const tetengo::lattice::entry_view& cpp_entry) {
                tetengo_lattice_entryView_t entry{};
                entry.key_handle = reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(cpp_entry.p_key());
                entry.value_handle = reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(cpp_entry.value());
                entry.cost = cpp_entry.cost();
                return p_entry_hash(&entry);
            },
            [p_entry_equal_to](
                const tetengo::lattice::entry_view& cpp_entry1, const tetengo::lattice::entry_view& cpp_entry2) {
                tetengo_lattice_entryView_t entry1{};
                entry1.key_handle = reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(cpp_entry1.p_key());
                entry1.value_handle = reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(cpp_entry1.value());
                entry1.cost = cpp_entry1.cost();

                tetengo_lattice_entryView_t entry2{};
                entry2.key_handle = reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(cpp_entry2.p_key());
                entry2.value_handle = reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(cpp_entry2.value());
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

namespace
{
    class custom_vocabulary : public tetengo::lattice::vocabulary
    {
    public:
        // constructors and destructor

        custom_vocabulary(const tetengo_lattice_customVocabularyDefinition_t& definition) : m_definition{ definition }
        {}


    private:
        // variables

        tetengo_lattice_customVocabularyDefinition_t m_definition;


        // virtual functions

        virtual std::vector<tetengo::lattice::entry_view>
        find_entries_impl(const tetengo::lattice::input& key) const override
        {
            const tetengo_lattice_input_t c_key{ key };
            const auto entry_count = m_definition.find_entries_proc(m_definition.p_context, &c_key, nullptr);
            if (entry_count == 0)
            {
                return std::vector<tetengo::lattice::entry_view>{};
            }

            std::vector<tetengo_lattice_entryView_t> c_entry_views(entry_count);
            const auto                               entry_count_again =
                m_definition.find_entries_proc(m_definition.p_context, &c_key, std::data(c_entry_views));
            if (entry_count_again != entry_count)
            {
                throw std::logic_error{ "Inconsistent entry count." };
            }

            std::vector<tetengo::lattice::entry_view> entries{};
            entries.reserve(entry_count);
            for (auto i = static_cast<std::size_t>(0); i < entry_count; ++i)
            {
                const auto&       c_entry_view = c_entry_views[i];
                const auto* const p_entry_key = tetengo_lattice_entryView_createKeyOf(c_entry_view.key_handle);
                BOOST_SCOPE_EXIT(p_entry_key)
                {
                    tetengo_lattice_input_destroy(p_entry_key);
                }
                BOOST_SCOPE_EXIT_END;
                entries.emplace_back(
                    p_entry_key ? &p_entry_key->cpp_input() : nullptr,
                    reinterpret_cast<const std::any*>(c_entry_view.value_handle),
                    c_entry_view.cost);
            }
            return entries;
        }

        virtual tetengo::lattice::connection
        find_connection_impl(const tetengo::lattice::node& from, const tetengo::lattice::entry_view& to) const override
        {
            if (from.value().type() != typeid(const void*))
            {
                throw std::invalid_argument{ "Unexcepted the value type of from." };
            }
            if (to.value()->type() != typeid(const void*))
            {
                throw std::invalid_argument{ "Unexcepted the value type of to." };
            }

            const tetengo_lattice_node_t c_from{ reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(from.p_key()),
                                                 reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(
                                                     &from.value()),
                                                 from.index_in_step(),
                                                 from.preceding_step(),
                                                 std::data(from.preceding_edge_costs()),
                                                 std::size(from.preceding_edge_costs()),
                                                 from.best_preceding_node(),
                                                 from.node_cost(),
                                                 from.path_cost() };
            const tetengo_lattice_entryView_t c_to{ reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(to.p_key()),
                                                    reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(
                                                        to.value()),
                                                    to.cost() };
            tetengo_lattice_connection_t      c_connection{};
            const auto                        result =
                m_definition.find_connection_proc(m_definition.p_context, &c_from, &c_to, &c_connection);
            if (!result)
            {
                throw std::runtime_error{ "Cannot obtain the connection." };
            }
            return tetengo::lattice::connection{ c_connection.cost };
        }
    };
}

tetengo_lattice_vocabulary_t* tetengo_lattice_vocabulary_createCustomVocabulary(
    const tetengo_lattice_customVocabularyDefinition_t* const p_definition)
{
    try
    {
        if (!p_definition)
        {
            throw std::invalid_argument{ "p_definition is NULL." };
        }

        auto p_cpp_vocabulary = std::make_unique<custom_vocabulary>(*p_definition);

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
    const tetengo_lattice_input_t* const      p_key,
    tetengo_lattice_entryView_t* const        p_entries)
{
    try
    {
        if (!p_vocabulary)
        {
            throw std::invalid_argument{ "p_vocabulary is NULL." };
        }
        if (!p_key)
        {
            throw std::invalid_argument{ "p_key is NULL." };
        }

        const auto found = p_vocabulary->p_cpp_vocabulary->find_entries(p_key->cpp_input());

        if (p_entries)
        {
            for (auto i = static_cast<std::size_t>(0); i < std::size(found); ++i)
            {
                const auto& cpp_entry = found[i];
                auto&       entry = p_entries[i];

                entry.key_handle = reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(cpp_entry.p_key());
                assert(cpp_entry.value()->type() == typeid(const void*));
                entry.value_handle = reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(cpp_entry.value());
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

bool tetengo_lattice_vocabulary_findConnection(
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

        const std::vector<int> cpp_preceding_edge_costs{};
        const auto* const      p_cpp_from_key = tetengo_lattice_entryView_createKeyOf(p_from->key_handle);
        BOOST_SCOPE_EXIT(p_cpp_from_key)
        {
            tetengo_lattice_input_destroy(p_cpp_from_key);
        }
        BOOST_SCOPE_EXIT_END;
        const tetengo::lattice::node cpp_from{ p_cpp_from_key ? &p_cpp_from_key->cpp_input() : nullptr,
                                               reinterpret_cast<const std::any*>(p_from->value_handle),
                                               p_from->index_in_step,
                                               p_from->preceding_step,
                                               &cpp_preceding_edge_costs,
                                               p_from->best_preceding_node,
                                               p_from->node_cost,
                                               p_from->path_cost };
        const auto* const p_cpp_to_key = tetengo_lattice_entryView_createKeyOf(p_to->key_handle);
        BOOST_SCOPE_EXIT(p_cpp_to_key)
        {
            tetengo_lattice_input_destroy(p_cpp_to_key);
        }
        BOOST_SCOPE_EXIT_END;
        const tetengo::lattice::entry_view cpp_to{ p_cpp_to_key ? &p_cpp_to_key->cpp_input() : nullptr,
                                                   reinterpret_cast<const std::any*>(p_to->value_handle),
                                                   p_to->cost };
        const auto found = p_vocabulary->p_cpp_vocabulary->find_connection(cpp_from, cpp_to);

        p_connection->cost = found.cost();

        return true;
    }
    catch (...)
    {
        return false;
    }
}
