/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
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
    namespace
    {
        std::optional<std::size_t>
        traverse(const storage& storage_, const std::size_t root_index, const std::string& key)
        {
            std::size_t index = root_index;
            for (const auto c: key)
            {
                const auto next_index =
                    static_cast<std::size_t>(storage_.base_at(index)) + static_cast<std::uint8_t>(c);
                if (next_index >= storage_.size() || storage_.check_at(next_index) != static_cast<std::uint8_t>(c))
                {
                    return std::nullopt;
                }
                index = next_index;
            }

            return std::make_optional(index);
        }

    }

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
    m_p_storage{ double_array_builder::build(
        std::vector<const std::pair<std::string, std::int32_t>*>{},
        null_building_observer(),
        default_density_factor()) },
        m_root_index{ 0 }
    {}

    double_array::double_array(
        std::vector<const std::pair<std::string, std::int32_t>*> element_pointers,
        const building_observer_type&                            building_observer /*= null_building_observer()*/,
        std::int32_t                                             density_factor /*= default_density_factor()*/) :
    m_p_storage{ double_array_builder::build(std::move(element_pointers), building_observer, density_factor) },
        m_root_index{ 0 }
    {}

    double_array::double_array(
        const std::vector<std::pair<std::string, std::int32_t>>& elements,
        const building_observer_type&                            building_observer /*= null_building_observer()*/,
        std::int32_t                                             density_factor /*= default_density_factor()*/) :
    m_p_storage{ double_array_builder::build(elements, building_observer, density_factor) },
        m_root_index{ 0 }
    {}

    double_array::double_array(std::unique_ptr<storage>&& p_storage) :
    m_p_storage{ std::move(p_storage) },
        m_root_index{ 0 }
    {}

    double_array::double_array(double_array&& another) :
    m_p_storage{ std::move(another.m_p_storage) },
        m_root_index{ another.m_root_index }
    {}

    double_array::double_array(const double_array& another) :
    m_p_storage{ another.m_p_storage->clone() },
        m_root_index{ another.m_root_index }
    {}

    double_array& double_array::operator=(double_array&& another)
    {
        m_p_storage = std::move(another).m_p_storage;
        m_root_index = another.m_root_index;
        return *this;
    }

    double_array& double_array::operator=(const double_array& another)
    {
        *this = double_array{ another };
        return *this;
    }

    std::optional<std::int32_t> double_array::find(const std::string& key) const
    {
        const auto o_index = traverse(*m_p_storage, m_root_index, key + double_array::key_terminator());
        return o_index ? std::make_optional(m_p_storage->base_at(*o_index)) : std::nullopt;
    }

    enumerator double_array::get_enumerator() const
    {
        return enumerator{ *m_p_storage, m_root_index };
    }

    std::optional<double_array> double_array::subtrie(const std::string& key_prefix) const
    {
        const auto o_index = traverse(*m_p_storage, m_root_index, key_prefix);
        return o_index ? std::make_optional(double_array{ *m_p_storage, *o_index }) : std::nullopt;
    }

    const storage& double_array::get_storage() const
    {
        return *m_p_storage;
    }

    double_array::double_array(const storage& storage_, std::size_t root_index) :
    m_p_storage{ storage_.clone() },
        m_root_index{ root_index }
    {}


}
