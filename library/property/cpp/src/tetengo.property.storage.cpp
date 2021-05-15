/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <type_traits>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    class storage::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl() {}


        // functions

        std::optional<std::uint32_t> get_uint32(const std::filesystem::path& /*key*/) const
        {
            return std::make_optional(42);
        }

        void set_uint32(const std::filesystem::path& /*key*/, const std::uint32_t /*value*/) {}

        void save(const storage& self) const
        {
            self.save_impl();
        }
    };


    storage::~storage() = default;

    std::optional<std::uint32_t> storage::get_uint32(const std::filesystem::path& key) const
    {
        return m_p_impl->get_uint32(key);
    }

    void storage::set_uint32(const std::filesystem::path& key, const std::uint32_t value)
    {
        m_p_impl->set_uint32(key, value);
    }

    void storage::save() const
    {
        return m_p_impl->save(*this);
    }

    storage::storage() : m_p_impl{ std::make_unique<impl>() } {}


    storage_loader::~storage_loader() = default;

    std::unique_ptr<storage> storage_loader::load(const std::filesystem::path& path) const
    {
        return load_impl(path);
    }


}
