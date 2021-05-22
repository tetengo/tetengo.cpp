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
    class windows_registry_reader::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(const std::filesystem::path& subkey) : m_handle{ open_registry_key(subkey) } {}

        ~impl()
        {
            ::RegCloseKey(m_handle);
        }


        // functions


    private:
        // static functions

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


        // variables

        const ::HKEY m_handle;
    };


    windows_registry_reader::windows_registry_reader(const std::filesystem::path& subkey) :
    m_p_impl{ std::make_unique<impl>(subkey) }
    {}

    windows_registry_reader::~windows_registry_reader() = default;


}
