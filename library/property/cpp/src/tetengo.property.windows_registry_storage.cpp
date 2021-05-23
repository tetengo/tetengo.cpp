/*! \file
    \brief A Windows registry storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if defined(_WIN32)

#include <filesystem>
#include <memory>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/platform_dependent/windows_X.hpp>
#include <tetengo/property/storage.hpp>
#include <tetengo/property/windows_registry_storage.hpp>


namespace tetengo::property
{
    class windows_registry_storage::impl : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = windows_registry_storage::value_map_type;


        // functions

        void save_impl(const windows_registry_storage& /*self*/) const {}


    private:
        // variables
    };


    windows_registry_storage::windows_registry_storage(value_map_type value_map) :
    storage{ std::move(value_map) },
        m_p_impl{ std::make_unique<impl>() }
    {}

    windows_registry_storage::~windows_registry_storage() = default;

    void windows_registry_storage::save_impl() const
    {
        m_p_impl->save_impl(*this);
    }


    class windows_registry_storage_loader::impl : private boost::noncopyable
    {
    public:
        // functions

        std::unique_ptr<storage> load_impl(const std::filesystem::path& path) const
        {
            value_map_type value_map{};
            load_impl_iter(path, std::filesystem::path{}, value_map);
            return std::make_unique<windows_registry_storage>(std::move(value_map));
        }


    private:
        // types

        using value_map_type = storage::value_map_type;


        // static functions

        static void load_impl_iter(
            const std::filesystem::path& path,
            const std::filesystem::path& relative_path,
            value_map_type&              value_map)
        {
            const tetengo::platform_dependent::windows_registry_reader reader{ path };
            for (const auto& value_name: reader.value_names())
            {
                const auto key = relative_path / value_name;
                if (const auto o_dword_value = reader.dword_value_of(value_name); o_dword_value)
                {
                    value_map.insert(std::make_pair(key.string(), *o_dword_value));
                }
                else if (const auto o_string_value = reader.string_value_of(value_name); o_string_value)
                {
                    value_map.insert(std::make_pair(key.string(), *o_string_value));
                }
            }
            for (const auto& child_subkey: reader.child_subkeys())
            {
                load_impl_iter(path / child_subkey, relative_path / child_subkey, value_map);
            }
        }
    };


    windows_registry_storage_loader::windows_registry_storage_loader() : m_p_impl{ std::make_unique<impl>() } {}

    windows_registry_storage_loader::~windows_registry_storage_loader() = default;

    std::unique_ptr<storage> windows_registry_storage_loader::load_impl(const std::filesystem::path& path) const
    {
        return m_p_impl->load_impl(path);
    }


}

#endif
