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
    namespace
    {
        static const std::hash<entry> entry_hash()
        {
            static const std::hash<entry> singleton{};
            return singleton;
        }


    }


    class unordered_map_vocabulary::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(
            std::unordered_map<std::string, std::vector<entry>> entry_map,
            std::unordered_map<std::pair<entry, entry>, int>    connection_map) :
        m_entry_map{ std::move(entry_map) },
            m_connection_map{ std::move(connection_map) }
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

        connection find_connection_impl(const node& from, const node& to) const
        {
            entry      entry_from{ std::string{ from.key() }, from.value(), from.node_cost() };
            entry      entry_to{ std::string{ to.key() }, to.value(), to.node_cost() };
            const auto found = m_connection_map.find(std::make_pair(std::move(entry_from), std::move(entry_to)));
            if (found == m_connection_map.end())
            {
                return connection{ std::numeric_limits<int>::max() };
            }
            return connection{ found->second };
        }


    private:
        // variables

        const std::unordered_map<std::string, std::vector<entry>> m_entry_map;

        const std::unordered_map<std::pair<entry, entry>, int> m_connection_map;
    };


    unordered_map_vocabulary::unordered_map_vocabulary(
        std::unordered_map<std::string, std::vector<entry>> entry_map,
        std::unordered_map<std::pair<entry, entry>, int>    connection_map) :
    m_p_impl{ std::make_unique<impl>(std::move(entry_map), std::move(connection_map)) }
    {}

    unordered_map_vocabulary::~unordered_map_vocabulary() = default;

    std::vector<entry_view> unordered_map_vocabulary::find_entries_impl(const std::string_view& key) const
    {
        return m_p_impl->find_entries_impl(key);
    }

    connection unordered_map_vocabulary::find_connection_impl(const node& from, const node& to) const
    {
        return m_p_impl->find_connection_impl(from, to);
    }


}


namespace std
{
    std::size_t hash<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>>::
                operator()(const std::pair<tetengo::lattice::entry, tetengo::lattice::entry>& key) const
    {
        auto seed = static_cast<std::size_t>(0);
        boost::hash_combine(seed, tetengo::lattice::entry_hash()(key.first));
        boost::hash_combine(seed, tetengo::lattice::entry_hash()(key.second));
        return seed;
    }


}
