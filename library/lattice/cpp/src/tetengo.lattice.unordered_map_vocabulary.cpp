/*! \file
    \brief An unordered_map vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>


namespace tetengo::lattice
{
    class unordered_map_vocabulary::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(const std::unordered_map<std::string_view, std::vector<entry>>& entries) :
        m_map{ to_holding_map(entries) }
        {}


        // functions

        std::vector<entry> find_impl(const std::string_view& key) const
        {
            const auto found = m_map.find(std::string{ key });
            if (found == m_map.end())
            {
                return std::vector<entry>{};
            }

            std::vector<entry> entries{};
            entries.reserve(found->second.size());
            std::transform(std::begin(found->second), std::end(found->second), std::back_inserter(entries), to_entry);
            return entries;
        }


    private:
        // types

        using entry_holder_type = std::tuple<std::string, std::string, int>;


        // static functions

        static std::unordered_map<std::string, std::vector<entry_holder_type>>
        to_holding_map(const std::unordered_map<std::string_view, std::vector<entry>>& entries)
        {
            std::unordered_map<std::string, std::vector<entry_holder_type>> map{};
            map.reserve(entries.size());
            std::transform(
                std::begin(entries), std::end(entries), std::inserter(map, map.end()), to_entry_holder_value);
            return map;
        }

        static std::pair<std::string, std::vector<entry_holder_type>>
        to_entry_holder_value(const std::pair<std::string_view, std::vector<entry>>& value)
        {
            std::vector<entry_holder_type> holding_entries{};
            holding_entries.reserve(value.second.size());
            std::transform(
                std::begin(value.second),
                std::end(value.second),
                std::back_inserter(holding_entries),
                to_holding_entry);
            return std::make_pair(std::string{ value.first }, std::move(holding_entries));
        }

        static entry_holder_type to_holding_entry(const entry& entry_)
        {
            return entry_holder_type{ entry_.key(), entry_.surface(), entry_.cost() };
        }

        static entry to_entry(const entry_holder_type& holder)
        {
            return entry{ std::get<0>(holder), std::get<1>(holder), std::get<2>(holder) };
        }


        // variables

        const std::unordered_map<std::string, std::vector<entry_holder_type>> m_map;
    };


    unordered_map_vocabulary::unordered_map_vocabulary(
        const std::unordered_map<std::string_view, std::vector<entry>>& entries) :
    m_p_impl{ std::make_unique<impl>(entries) }
    {}

    unordered_map_vocabulary::~unordered_map_vocabulary() = default;

    std::vector<entry> unordered_map_vocabulary::find_impl(const std::string_view& key) const
    {
        return m_p_impl->find_impl(key);
    }


}
