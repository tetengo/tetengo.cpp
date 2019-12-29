/*! \file
    \brief A trie.

    Copyright (C) 2019 kaoru
*/

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/trie.hpp>


namespace tetengo::trie
{
    class trie_impl::impl : private boost::noncopyable
    {
    public:
        // types

        using key_type = trie_impl::key_type;

        using mapped_type = trie_impl::mapped_type;

        using value_type = trie_impl::value_type;

        using building_observer_set_type = trie_impl::building_observer_set_type;


        // static functions

        static const building_observer_set_type& null_building_observer_set()
        {
            static const building_observer_set_type singleton{ [](const std::string_view&) {}, []() {} };
            return singleton;
        }

        static std::size_t default_double_array_density_factor()
        {
            return double_array::default_density_factor();
        }


        // constructors and destructor

        impl() : m_p_double_array{ std::make_unique<double_array>() } {}

        impl(
            std::vector<value_type>           elements,
            const building_observer_set_type& building_observer_set,
            const std::size_t                 double_array_density_factor) :
        m_p_double_array{}
        {
            std::vector<std::pair<std::string_view, std::int32_t>> double_array_contents{};
            double_array_contents.reserve(elements.size());
            for (auto i = static_cast<std::int32_t>(0); i < static_cast<std::int32_t>(elements.size()); ++i)
            {
                double_array_contents.emplace_back(std::move(elements[i].first), i);
            }

            const double_array::building_observer_set_type double_array_building_observer_set{
                [&building_observer_set](const std::pair<std::string_view, std::int32_t>& element) {
                    building_observer_set.adding(element.first);
                },
                [&building_observer_set]() { building_observer_set.done(); }
            };

            m_p_double_array = std::make_unique<double_array>(
                double_array_contents, double_array_building_observer_set, double_array_density_factor);

            for (auto i = static_cast<std::int32_t>(0); i < static_cast<std::int32_t>(elements.size()); ++i)
            {
                m_p_double_array->get_storage().add_mapped_at(i, std::move(elements[i].second));
            }
        }


    private:
        // variables

        std::unique_ptr<double_array> m_p_double_array;
    };


    const trie_impl::building_observer_set_type& trie_impl::null_building_observer_set()
    {
        return impl::null_building_observer_set();
    }

    std::size_t trie_impl::default_double_array_density_factor()
    {
        return impl::default_double_array_density_factor();
    }

    trie_impl::trie_impl() : m_p_impl{ std::make_unique<impl>() } {}

    trie_impl::trie_impl(
        std::vector<value_type>           elements,
        const building_observer_set_type& building_observer_set,
        const std::size_t                 double_array_density_factor) :
    m_p_impl{ std::make_unique<impl>(std::move(elements), building_observer_set, double_array_density_factor) }
    {}

    trie_impl::~trie_impl() = default;


}
