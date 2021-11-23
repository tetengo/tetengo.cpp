/*! \file
    \brief An unordered_map vocabulary.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/lattice/connection.hpp>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/input.hpp>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/string_input.hpp>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>


namespace tetengo::lattice
{
    class unordered_map_vocabulary::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl(
            std::vector<std::pair<std::string, std::vector<entry>>>   entries,
            std::vector<std::pair<std::pair<entry, entry>, int>>      connections,
            std::function<std::size_t(const entry_view&)>             entry_hash,
            std::function<bool(const entry_view&, const entry_view&)> entry_equal_to) :
        m_entry_map{ make_entry_map(std::move(entries)) },
        m_connection_keys{},
        m_p_connection_map{}
        {
            build_connection_map(
                std::move(connections),
                std::move(entry_hash),
                std::move(entry_equal_to),
                m_connection_keys,
                m_p_connection_map);
        }


        // functions

        std::vector<entry_view> find_entries_impl(const input_base& key) const
        {
            const auto found = m_entry_map.find(std::string{ key.as<string_input>().value() });
            if (found == std::end(m_entry_map))
            {
                return std::vector<entry_view>{};
            }

            std::vector<entry_view> entries{};
            entries.reserve(std::size(found->second));
            std::copy(std::begin(found->second), std::end(found->second), std::back_inserter(entries));
            return entries;
        }

        connection find_connection_impl(const node& from, const entry_view& to) const
        {
            const entry_view from_entry_view{ from.key(), &from.value(), from.node_cost() };
            const auto       found = m_p_connection_map->find(std::make_pair(from_entry_view, to));
            if (found == std::end(*m_p_connection_map))
            {
                return connection{ std::numeric_limits<int>::max() };
            }
            return connection{ found->second };
        }


    private:
        // types

        using entry_map_type = std::unordered_map<std::string, std::vector<entry>>;

        struct connection_map_hash
        {
            std::function<std::size_t(const entry_view&)> hash;

            explicit connection_map_hash(std::function<std::size_t(const entry_view&)> hash) : hash{ std::move(hash) }
            {}

            std::size_t operator()(const std::pair<entry_view, entry_view>& key) const
            {
                return hash(key.first) ^ hash(key.second);
            }
        };

        struct connection_map_key_eq
        {
            std::function<bool(const entry_view&, const entry_view&)> key_eq;

            explicit connection_map_key_eq(std::function<bool(const entry_view&, const entry_view&)> key_eq) :
            key_eq{ std::move(key_eq) }
            {}

            bool operator()(
                const std::pair<entry_view, entry_view>& one,
                const std::pair<entry_view, entry_view>& another) const
            {
                return key_eq(one.first, another.first) && key_eq(one.second, another.second);
            }
        };

        using connection_map_type =
            std::unordered_map<std::pair<entry_view, entry_view>, int, connection_map_hash, connection_map_key_eq>;


        // static functions

        static entry_map_type make_entry_map(std::vector<std::pair<std::string, std::vector<entry>>> entries)
        {
            entry_map_type map{};
            map.reserve(std::size(entries));
            for (auto&& e: entries)
            {
                map.insert(std::make_pair(std::move(e.first), std::move(e.second)));
            }
            return map;
        }

        static void build_connection_map(
            std::vector<std::pair<std::pair<entry, entry>, int>>      connections,
            std::function<std::size_t(const entry_view&)>             entry_hash,
            std::function<bool(const entry_view&, const entry_view&)> entry_equal_to,
            std::vector<std::pair<entry, entry>>&                     connection_keys,
            std::unique_ptr<connection_map_type>&                     p_connection_map)
        {
            connection_keys.reserve(std::size(connections));
            for (auto&& e: connections)
            {
                connection_keys.push_back(std::move(e.first));
            }

            auto p_map = std::make_unique<connection_map_type>(
                std::size(connections),
                connection_map_hash{ std::move(entry_hash) },
                connection_map_key_eq{ std::move(entry_equal_to) });
            p_map->reserve(std::size(connections));
            for (auto i = static_cast<std::size_t>(0); i < std::size(connections); ++i)
            {
                const auto&      connection_key = connection_keys[i];
                const entry_view from{ connection_key.first.key(),
                                       &connection_key.first.value(),
                                       connection_key.first.cost() };
                const entry_view to{ connection_key.second.key(),
                                     &connection_key.second.value(),
                                     connection_key.second.cost() };
                p_map->insert(std::make_pair(std::make_pair(from, to), connections[i].second));
            }

            p_connection_map = std::move(p_map);
        }


        // variables

        const entry_map_type m_entry_map;

        std::vector<std::pair<entry, entry>> m_connection_keys;

        std::unique_ptr<connection_map_type> m_p_connection_map;
    };


    unordered_map_vocabulary::unordered_map_vocabulary(
        std::vector<std::pair<std::string, std::vector<entry>>>   entries,
        std::vector<std::pair<std::pair<entry, entry>, int>>      connections,
        std::function<std::size_t(const entry_view&)>             entry_hash,
        std::function<bool(const entry_view&, const entry_view&)> entry_equal_to) :
    m_p_impl{ std::make_unique<impl>(
        std::move(entries),
        std::move(connections),
        std::move(entry_hash),
        std::move(entry_equal_to)) }
    {}

    unordered_map_vocabulary::~unordered_map_vocabulary() = default;

    std::vector<entry_view> unordered_map_vocabulary::find_entries_impl(const input_base& key) const
    {
        return m_p_impl->find_entries_impl(key);
    }

    connection unordered_map_vocabulary::find_connection_impl(const node& from, const entry_view& to) const
    {
        return m_p_impl->find_connection_impl(from, to);
    }


}
