/*! \file
    \brief An unordered_map vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <string_view>
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

        impl() {}


        // functions

        std::vector<entry> find_impl(const std::string_view& /*key*/) const
        {
            return std::vector<entry>{};
        }
    };


    unordered_map_vocabulary::unordered_map_vocabulary() : m_p_impl{ std::make_unique<impl>() } {}

    unordered_map_vocabulary::~unordered_map_vocabulary() = default;

    std::vector<entry> unordered_map_vocabulary::find_impl(const std::string_view& key) const
    {
        return m_p_impl->find_impl(key);
    }


}
