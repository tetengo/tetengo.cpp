/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/enumerator.hpp>
#include <tetengo/trie/storage.hpp>

#include "tetengo.trie.double_array_builder.hpp"


namespace tetengo::trie
{
    const double_array::building_observer_type& double_array::null_building_observer()
    {
        static const building_observer_type singleton{ [](const std::string&) {}, []() {} };
        return singleton;
    }

    std::int32_t double_array::default_density_factor()
    {
        return double_array_builder::default_density_factor();
    }

    double_array::double_array() :
    m_storage{ double_array_builder::build(
        std::vector<const std::pair<std::string, std::int32_t>*>{},
        null_building_observer(),
        default_density_factor()) }
    {}

    double_array::double_array(
        std::vector<const std::pair<std::string, std::int32_t>*> element_pointers,
        const building_observer_type&                            building_observer /*= null_building_observer()*/,
        std::int32_t                                             density_factor /*= default_density_factor()*/) :
    m_storage{ double_array_builder::build(std::move(element_pointers), building_observer, density_factor) }
    {}

    double_array::double_array(
        const std::vector<std::pair<std::string, std::int32_t>>& elements,
        const building_observer_type&                            building_observer /*= null_building_observer()*/,
        std::int32_t                                             density_factor /*= default_density_factor()*/) :
    m_storage{ double_array_builder::build(elements, building_observer, density_factor) }
    {}

    double_array::double_array(storage&& storage_) : m_storage{ std::move(storage_) } {}

    std::optional<std::int32_t> double_array::find(const std::string& key) const
    {
        std::size_t index = 0;
        for (const auto c: key + double_array::key_terminator())
        {
            const auto next_index = static_cast<std::size_t>(m_storage.base_at(index)) + static_cast<std::uint8_t>(c);
            if (next_index >= m_storage.size() || m_storage.check_at(next_index) != static_cast<std::uint8_t>(c))
            {
                return std::nullopt;
            }
            index = next_index;
        }

        return std::make_optional(m_storage.base_at(index));
    }

    enumerator double_array::get_enumerator() const
    {
        return enumerator{ m_storage };
    }

    double_array double_array::subtrie(const std::string& /*key_prefix*/) const
    {
        return double_array{};
    }

    const storage& double_array::get_storage() const
    {
        return m_storage;
    }


}
