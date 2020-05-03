/*! \file
    \brief An unordered_map vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cstddef>
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
            std::vector<std::pair<std::pair<entry, entry>, int>>    connections) :
        m_entry_map{ make_entry_map(std::move(entries)) },
            m_connection_map{ make_connection_map(std::move(connections)) }
        {}


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
            const auto found =
                m_connection_map.find(std::make_pair(std::string{ from.key() }, std::string{ to.key() }));
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
            std::size_t operator()(const std::pair<std::string, std::string>& key) const
            {
                return boost::hash_value(key);
            }
        };

        using connection_map_type = std::unordered_map<std::pair<std::string, std::string>, int, connection_map_hash>;


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

        static connection_map_type make_connection_map(std::vector<std::pair<std::pair<entry, entry>, int>> connections)
        {
            connection_map_type map{};
            map.reserve(connections.size());
            for (auto&& e: connections)
            {
                map.insert(std::make_pair(std::make_pair(e.first.first.key(), e.first.second.key()), e.second));
            }
            return map;
        }


        // variables

        const entry_map_type m_entry_map;

        const connection_map_type m_connection_map;
    };


    unordered_map_vocabulary::unordered_map_vocabulary(
        std::vector<std::pair<std::string, std::vector<entry>>> entries,
        std::vector<std::pair<std::pair<entry, entry>, int>>    connections) :
    m_p_impl{ std::make_unique<impl>(std::move(entries), std::move(connections)) }
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
