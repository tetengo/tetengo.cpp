/*! \file
    \brief A storage.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <filesystem>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <type_traits> // IWYU pragma: keep
#include <unordered_map>
#include <utility>
#include <variant>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    class storage::impl : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = storage::value_map_type;


        // constructors and destructor

        explicit impl(value_map_type value_map) : m_value_map{ std::move(value_map) } {}


        // functions

        std::optional<bool> get_bool(const std::filesystem::path& key, const storage& self) const
        {
            return self.get_bool_impl(key);
        }

        void set_bool(const std::filesystem::path& key, const bool value, storage& self)
        {
            self.set_bool_impl(key, value);
        }

        std::optional<std::uint32_t> get_uint32(const std::filesystem::path& key, const storage& self) const
        {
            return self.get_uint32_impl(key);
        }

        void set_uint32(const std::filesystem::path& key, const std::uint32_t value, storage& self)
        {
            self.set_uint32_impl(key, value);
        }

        std::optional<std::string> get_string(const std::filesystem::path& key, const storage& self) const
        {
            return self.get_string_impl(key);
        }

        void set_string(const std::filesystem::path& key, const std::string& value, storage& self)
        {
            self.set_string_impl(key, value);
        }

        void save(const storage& self) const
        {
            self.save_impl();
        }

        const value_map_type& value_map() const
        {
            return m_value_map;
        }

        std::optional<bool> get_bool_impl(const std::filesystem::path& key) const
        {
            const auto found = m_value_map.find(key.string());
            if (found == std::end(m_value_map))
            {
                return std::nullopt;
            }
            if (std::holds_alternative<bool>(found->second))
            {
                return std::get<bool>(found->second);
            }
            else if (std::holds_alternative<std::uint32_t>(found->second))
            {
                return std::get<std::uint32_t>(found->second);
            }
            else
            {
                return std::nullopt;
            }
        }

        void set_bool_impl(const std::filesystem::path& key, bool value)
        {
            m_value_map.insert(std::make_pair(key.string(), value));
        }

        std::optional<std::uint32_t> get_uint32_impl(const std::filesystem::path& key) const
        {
            const auto found = m_value_map.find(key.string());
            if (found == std::end(m_value_map))
            {
                return std::nullopt;
            }
            if (std::holds_alternative<std::uint32_t>(found->second))
            {
                return std::get<std::uint32_t>(found->second);
            }
            else
            {
                return std::nullopt;
            }
        }

        void set_uint32_impl(const std::filesystem::path& key, std::uint32_t value)
        {
            m_value_map.insert(std::make_pair(key.string(), value));
        }

        std::optional<std::string> get_string_impl(const std::filesystem::path& key) const
        {
            const auto found = m_value_map.find(key.string());
            if (found == std::end(m_value_map))
            {
                return std::nullopt;
            }
            if (std::holds_alternative<std::string>(found->second))
            {
                return std::get<std::string>(found->second);
            }
            else
            {
                return std::nullopt;
            }
        }

        void set_string_impl(const std::filesystem::path& key, const std::string& value)
        {
            m_value_map.insert(std::make_pair(key.string(), value));
        }


    private:
        value_map_type m_value_map;
    };


    storage::~storage() = default;

    std::optional<bool> storage::get_bool(const std::filesystem::path& key) const
    {
        return m_p_impl->get_bool(key, *this);
    }

    void storage::set_bool(const std::filesystem::path& key, const bool value)
    {
        m_p_impl->set_bool(key, value, *this);
    }

    std::optional<std::uint32_t> storage::get_uint32(const std::filesystem::path& key) const
    {
        return m_p_impl->get_uint32(key, *this);
    }

    void storage::set_uint32(const std::filesystem::path& key, const std::uint32_t value)
    {
        m_p_impl->set_uint32(key, value, *this);
    }

    std::optional<std::string> storage::get_string(const std::filesystem::path& key) const
    {
        return m_p_impl->get_string(key, *this);
    }

    void storage::set_string(const std::filesystem::path& key, const std::string& value)
    {
        m_p_impl->set_string(key, value, *this);
    }

    void storage::save() const
    {
        return m_p_impl->save(*this);
    }

    storage::storage(value_map_type value_map) : m_p_impl{ std::make_unique<impl>(std::move(value_map)) } {}

    const storage::value_map_type& storage::value_map() const
    {
        return m_p_impl->value_map();
    }

    std::optional<bool> storage::get_bool_impl(const std::filesystem::path& key) const
    {
        return m_p_impl->get_bool_impl(key);
    }

    void storage::set_bool_impl(const std::filesystem::path& key, bool value)
    {
        m_p_impl->set_bool_impl(key, value);
    }

    std::optional<std::uint32_t> storage::get_uint32_impl(const std::filesystem::path& key) const
    {
        return m_p_impl->get_uint32_impl(key);
    }

    void storage::set_uint32_impl(const std::filesystem::path& key, std::uint32_t value)
    {
        m_p_impl->set_uint32_impl(key, value);
    }

    std::optional<std::string> storage::get_string_impl(const std::filesystem::path& key) const
    {
        return m_p_impl->get_string_impl(key);
    }

    void storage::set_string_impl(const std::filesystem::path& key, const std::string& value)
    {
        m_p_impl->set_string_impl(key, value);
    }


    storage_loader::~storage_loader() = default;

    std::unique_ptr<storage> storage_loader::load(const std::filesystem::path& path) const
    {
        return load_impl(path);
    }


}
