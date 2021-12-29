/*! \file
    \brief A property set file path.

    For UNIX.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <cstdlib>
#include <filesystem>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string_view>

#include <boost/core/noncopyable.hpp>

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

            const auto native_path = home_path() / add_extension(add_prefix(generic_path, "."), ".json");
            return native_path;
        }

        std::filesystem::path to_native_top_path(const std::filesystem::path& generic_path) const
        {
            if (std::empty(generic_path))
            {
                throw std::invalid_argument{ "generic_path is empty." };
            }

            const auto native_path = home_path() / top_path(generic_path);
            return native_path;
        }


    private:
        // static functions

        static std::filesystem::path home_path()
        {
            const auto* const p_home_path = std::getenv("HOME");
            if (!p_home_path)
            {
                throw std::runtime_error{ "Can't obtain the home directory path." };
            }
            return std::filesystem::path{ p_home_path };
        }

        static std::filesystem::path add_prefix(const std::filesystem::path& path, const std::string_view& prefix)
        {
            std::filesystem::path prefixed{ prefix };
            prefixed += path;
            return prefixed;
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
