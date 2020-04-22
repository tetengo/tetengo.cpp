/*! \file
    \brief An unordered_map vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
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

        explicit impl(std::unordered_map<std::string, std::vector<entry>> map) : m_map{ std::move(map) } {}


        // functions

        std::vector<entry_view> find_impl(const std::string_view& key) const
        {
            const auto found = m_map.find(std::string{ key });
            if (found == m_map.end())
            {
                return std::vector<entry_view>{};
            }

            std::vector<entry_view> entries{};
            entries.reserve(found->second.size());
            std::copy(std::begin(found->second), std::end(found->second), std::back_inserter(entries));
            return entries;
        }


    private:
        // variables

        const std::unordered_map<std::string, std::vector<entry>> m_map;
    };


    unordered_map_vocabulary::unordered_map_vocabulary(std::unordered_map<std::string, std::vector<entry>> map) :
    m_p_impl{ std::make_unique<impl>(std::move(map)) }
    {}

    unordered_map_vocabulary::~unordered_map_vocabulary() = default;

    std::vector<entry_view> unordered_map_vocabulary::find_impl(const std::string_view& key) const
    {
        return m_p_impl->find_impl(key);
    }


}
