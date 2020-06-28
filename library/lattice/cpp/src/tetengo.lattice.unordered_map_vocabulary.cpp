/*! \file
    \brief An unordered_map vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/container_hash/hash.hpp>
#include <boost/core/noncopyable.hpp>

#include <tetengo/lattice/connection.hpp>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>


namespace tetengo::lattice
{
    class unordered_map_vocabulary::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl(
            std::vector<std::pair<std::string, std::vector<entry>>> entries,
            std::vector<std::pair<std::pair<entry, entry>, int>>    connections,
            std::function<std::size_t(const entry_view&)>           entry_hash) :
        m_entry_map{ make_entry_map(std::move(entries)) },
            m_connection_keys{},
            m_connection_map{}
        {
            build_connection_map(std::move(connections), entry_hash, m_connection_keys, m_connection_map);
        }


        // functions

        std::vector<entry_view> find_entries_impl(const std::string_view& key) const
        {
            const auto found = m_entry_map.find(std::string{ key });
            if (found == m_entry_map.end())
            {
                return std::vector<entry_view>{};
            }

            std::vector<entry_view> entries{};
            entries.reserve(found->second.size());
            std::copy(std::begin(found->second), std::end(found->second), std::back_inserter(entries));
            return entries;
        }

        connection find_connection_impl(const node& from, const entry_view& to) const
        {
            const entry_view from_entry_view{ from.key(), &from.value(), from.node_cost() };
            const auto       found = m_connection_map.find(std::make_pair(from_entry_view, to));
            if (found == m_connection_map.end())
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
            std::size_t operator()(const std::pair<entry_view, entry_view>& key) const
            {
                return boost::hash_value(std::make_pair(key.first.key(), key.second.key()));
            }
        };

        struct connection_map_key_eq
        {
            bool operator()(
                const std::pair<entry_view, entry_view>& one,
                const std::pair<entry_view, entry_view>& another) const
            {
                return one.first.key() == another.first.key() && one.second.key() == another.second.key();
            }
        };

        using connection_map_type =
            std::unordered_map<std::pair<entry_view, entry_view>, int, connection_map_hash, connection_map_key_eq>;


        // static functions

        static entry_map_type make_entry_map(std::vector<std::pair<std::string, std::vector<entry>>> entries)
        {
            entry_map_type map{};
            map.reserve(entries.size());
            for (auto&& e: entries)
            {
                map.insert(std::make_pair(std::move(e.first), std::move(e.second)));
            }
            return map;
        }

        static void build_connection_map(
            std::vector<std::pair<std::pair<entry, entry>, int>> connections,
            std::function<std::size_t(const entry_view&)> /*entry_hash*/,
            std::vector<std::pair<entry, entry>>& connection_keys,
            connection_map_type&                  connection_map)
        {
            connection_keys.reserve(connections.size());
            for (auto&& e: connections)
            {
                connection_keys.push_back(std::move(e.first));
            }

            connection_map_type map{};
            map.reserve(connections.size());
            for (auto i = static_cast<std::size_t>(0); i < connections.size(); ++i)
            {
                const auto&      connection_key = connection_keys[i];
                const entry_view from{ connection_key.first.key(),
                                       &connection_key.first.value(),
                                       connection_key.first.cost() };
                const entry_view to{ connection_key.second.key(),
                                     &connection_key.second.value(),
                                     connection_key.second.cost() };
                map.insert(std::make_pair(std::make_pair(from, to), connections[i].second));
            }

            connection_map = std::move(map);
        }


        // variables

        const entry_map_type m_entry_map;

        std::vector<std::pair<entry, entry>> m_connection_keys;

        connection_map_type m_connection_map;
    };


    unordered_map_vocabulary::unordered_map_vocabulary(
        std::vector<std::pair<std::string, std::vector<entry>>> entries,
        std::vector<std::pair<std::pair<entry, entry>, int>>    connections,
        std::function<std::size_t(const entry_view&)>           entry_hash) :
    m_p_impl{ std::make_unique<impl>(std::move(entries), std::move(connections), std::move(entry_hash)) }
    {}

    unordered_map_vocabulary::~unordered_map_vocabulary() = default;

    std::vector<entry_view> unordered_map_vocabulary::find_entries_impl(const std::string_view& key) const
    {
        return m_p_impl->find_entries_impl(key);
    }

    connection unordered_map_vocabulary::find_connection_impl(const node& from, const entry_view& to) const
    {
        return m_p_impl->find_connection_impl(from, to);
    }


}
