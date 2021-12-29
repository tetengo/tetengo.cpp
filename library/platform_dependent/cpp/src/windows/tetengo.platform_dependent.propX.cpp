/*! \file
    \brief A property set file path.

    For Windows.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <iterator>
#include <memory>
#include <stdexcept>

#include <ShlObj.h>
#include <Windows.h>

#include <boost/core/noncopyable.hpp>
#include <boost/scope_exit.hpp>

#include <tetengo/platform_dependent/propertyX.hpp>


namespace tetengo::platform_dependent
{
    class property_set_file_path::impl : private boost::noncopyable
    {
    public:
        // static functions

        static const property_set_file_path& instance()
        {
            static const property_set_file_path singleton{};
            return singleton;
        }


        // functions

        std::filesystem::path to_native_path(const std::filesystem::path& generic_path) const
        {
            if (std::empty(generic_path))
            {
                throw std::invalid_argument{ "generic_path is empty." };
            }

            if (generic_path.has_parent_path())
            {
                auto native_path = appdata_path() / add_extension(generic_path, ".json");
                return native_path;
            }
            else
            {
                auto native_path = appdata_path() / generic_path / add_extension(generic_path, ".json");
                return native_path;
            }
        }

        std::filesystem::path to_native_top_path(const std::filesystem::path& generic_path) const
        {
            if (std::empty(generic_path))
            {
                throw std::invalid_argument{ "generic_path is empty." };
            }

            auto native_path = appdata_path() / top_path(generic_path);
            return native_path;
        }


    private:
        // static functions

        static std::filesystem::path appdata_path()
        {
            PWSTR      p_appdata_path = nullptr;
            const auto result =
                ::SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, nullptr, &p_appdata_path);
            BOOST_SCOPE_EXIT(p_appdata_path)
            {
                ::CoTaskMemFree(p_appdata_path);
            }
            BOOST_SCOPE_EXIT_END;
            if (result != S_OK)
            {
                throw std::runtime_error{ "Can't obtain the application data folder path." };
            }
            return std::filesystem::path{ p_appdata_path };
        }

        static std::filesystem::path add_extension(std::filesystem::path path, const std::string_view& extension)
        {
            path += extension;
            return path;
        }

        static std::filesystem::path top_path(const std::filesystem::path& path)
        {
            if (!path.has_parent_path())
            {
                return path;
            }
            return top_path(path.parent_path());
        }
    };


    const property_set_file_path& property_set_file_path::instance()
    {
        return impl::instance();
    }

    property_set_file_path::~property_set_file_path() = default;

    std::filesystem::path property_set_file_path::to_native_path(const std::filesystem::path& generic_path) const
    {
        return m_p_impl->to_native_path(generic_path);
    }

    std::filesystem::path property_set_file_path::to_native_top_path(const std::filesystem::path& generic_path) const
    {
        return m_p_impl->to_native_top_path(generic_path);
    }

    property_set_file_path::property_set_file_path() : m_p_impl{ std::make_unique<impl>() } {}
}
