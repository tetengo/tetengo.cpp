/*! \file
    \brief A Windows registry access.

    For Windows.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <stdexcept>

#include <Windows.h>

#include <boost/core/noncopyable.hpp>

#include <tetengo/platform_dependent/windows_X.hpp>


namespace tetengo::platform_dependent
{
    class windows_registry::impl : private boost::noncopyable
    {
    public:
        // types

        using open_mode_type = windows_registry::open_mode_type;


        // constructors and destructor

        impl(const std::filesystem::path& subkey, const open_mode_type open_mode) :
        m_handle{ open_or_create_registry_key(subkey, open_mode) }
        {}

        ~impl()
        {
            ::RegCloseKey(m_handle);
        }


        // functions


    private:
        // static functions

        static ::HKEY open_or_create_registry_key(const std::filesystem::path& subkey, const open_mode_type open_mode)
        {
            return open_mode == open_mode_type::read ? open_registry_key(subkey) : create_registry_key(subkey);
        }

        static ::HKEY open_registry_key(const std::filesystem::path& subkey)
        {
            const auto native_subkey = "SOFTWARE" / subkey;
            ::HKEY     handle = nullptr;
            const auto result = ::RegOpenKeyExW(HKEY_CURRENT_USER, native_subkey.c_str(), 0, KEY_READ, &handle);
            if (result != ERROR_SUCCESS)
            {
                throw std::runtime_error{ "Can't open the Windows registry key." };
            }
            return handle;
        }

        static ::HKEY create_registry_key(const std::filesystem::path& /*subkey*/)
        {
            return nullptr;
        }


        // variables

        const ::HKEY m_handle;
    };


    windows_registry::windows_registry(const std::filesystem::path& subkey, const open_mode_type open_mode) :
    m_p_impl{ std::make_unique<impl>(subkey, open_mode) }
    {}

    windows_registry::~windows_registry() = default;


}
