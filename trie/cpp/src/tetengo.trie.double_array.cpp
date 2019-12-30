/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/enumerator.hpp>
#include <tetengo/trie/storage.hpp>

#include "tetengo.trie.double_array_builder.hpp"


namespace tetengo::trie
{
    class double_array::impl
    {
    public:
        // tyes

        using building_observer_set_type = double_array::building_observer_set_type;


        // static functions

        static const building_observer_set_type& null_building_observer_set()
        {
            static const building_observer_set_type singleton{ [](const std::pair<std::string_view, std::int32_t>&) {},
                                                               []() {} };
            return singleton;
        }

        static std::size_t default_density_factor()
        {
            return double_array_builder::default_density_factor();
        }

        static constexpr char key_terminator()
        {
            return '\0';
        }

        static constexpr std::uint8_t vacant_check_value()
        {
            return 0xFF;
        }


        // constructors and destructor

        impl() :
        m_p_storage{ double_array_builder::build(
            std::vector<std::pair<std::string_view, std::int32_t>>{},
            null_building_observer_set(),
            default_density_factor()) },
            m_root_base_check_index{ 0 }
        {}

        impl(
            const std::vector<std::pair<std::string_view, std::int32_t>>& elements,
            const building_observer_set_type&                             building_observer_set,
            const std::size_t                                             density_factor) :
        m_p_storage{ double_array_builder::build(elements, building_observer_set, density_factor) },
            m_root_base_check_index{ 0 }
        {}

        impl(
            const std::vector<std::pair<std::string, std::int32_t>>& elements,
            const building_observer_set_type&                        building_observer_set,
            const std::size_t                                        density_factor) :
        impl{ std::vector<std::pair<std::string_view, std::int32_t>>{ std::begin(elements), std::end(elements) },
              building_observer_set,
              density_factor }
        {}

        impl(std::unique_ptr<storage>&& p_storage, std::size_t root_base_check_index) :
        m_p_storage{ std::move(p_storage) },
            m_root_base_check_index{ root_base_check_index }
        {}


        // functions

        std::optional<std::int32_t> find(const std::string_view& key) const
        {
            const auto o_index = traverse(std::string{ key } + double_array::key_terminator());
            return o_index ? std::make_optional(m_p_storage->base_at(*o_index)) : std::nullopt;
        }

        enumerator get_enumerator() const
        {
            return enumerator{ *m_p_storage, m_root_base_check_index };
        }

        std::unique_ptr<double_array> subtrie(const std::string_view& key_prefix) const
        {
            const auto o_index = traverse(key_prefix);
            return o_index ? std::make_unique<double_array>(m_p_storage->clone(), *o_index) :
                             std::unique_ptr<double_array>{};
        }

        const storage& get_storage() const
        {
            return *m_p_storage;
        }

        storage& get_storage()
        {
            return *m_p_storage;
        }


    private:
        // variables

        std::unique_ptr<storage> m_p_storage;

        std::size_t m_root_base_check_index;


        // functions

        std::optional<std::size_t> traverse(const std::string_view& key) const
        {
            auto base_check_index = m_root_base_check_index;
            for (const auto c: key)
            {
                const auto next_base_check_index =
                    static_cast<std::size_t>(m_p_storage->base_at(base_check_index)) + static_cast<std::uint8_t>(c);
                if (next_base_check_index >= m_p_storage->base_check_array().size() ||
                    m_p_storage->check_at(next_base_check_index) != static_cast<std::uint8_t>(c))
                {
                    return std::nullopt;
                }
                base_check_index = next_base_check_index;
            }

            return std::make_optional(base_check_index);
        }
    };


    const double_array::building_observer_set_type& double_array::null_building_observer_set()
    {
        return impl::null_building_observer_set();
    }

    std::size_t double_array::default_density_factor()
    {
        return impl::default_density_factor();
    }

    double_array::double_array() : m_p_impl{ std::make_unique<impl>() } {}

    double_array::double_array(
        const std::vector<std::pair<std::string_view, std::int32_t>>& elements,
        const building_observer_set_type& building_observer_set /*= null_building_observer_set()*/,
        std::size_t                       density_factor /*= default_density_factor()*/) :
    m_p_impl{ std::make_unique<impl>(elements, building_observer_set, density_factor) }
    {}

    double_array::double_array(
        const std::vector<std::pair<std::string, std::int32_t>>& elements,
        const building_observer_set_type& building_observer_set /*= null_building_observer_set()*/,
        std::size_t                       density_factor /*= default_density_factor()*/) :
    m_p_impl{ std::make_unique<impl>(elements, building_observer_set, density_factor) }
    {}

    double_array::double_array(std::unique_ptr<storage>&& p_storage, const std::size_t root_base_check_index) :
    m_p_impl{ std::make_unique<impl>(std::move(p_storage), root_base_check_index) }
    {}

    double_array::~double_array() = default;

    std::optional<std::int32_t> double_array::find(const std::string_view& key) const
    {
        return m_p_impl->find(key);
    }

    enumerator double_array::get_enumerator() const
    {
        return m_p_impl->get_enumerator();
    }

    std::unique_ptr<double_array> double_array::subtrie(const std::string_view& key_prefix) const
    {
        return m_p_impl->subtrie(key_prefix);
    }

    const storage& double_array::get_storage() const
    {
        return m_p_impl->get_storage();
    }

    storage& double_array::get_storage()
    {
        return m_p_impl->get_storage();
    }


}
