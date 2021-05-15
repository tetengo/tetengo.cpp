/*! \file
    \brief A property set file path.

    For UNIX.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <memory>
#include <stdexcept>

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
            if (generic_path.empty())
            {
                throw std::invalid_argument{ "generic_path is empty." };
            }

            return generic_path;
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

    property_set_file_path::property_set_file_path() : m_p_impl{ std::make_unique<impl>() } {}
}
