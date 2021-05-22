/*! \file
    \brief A Windows registry access.

    For Windows.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

#include <Windows.h>

#include <boost/core/noncopyable.hpp>

#include <tetengo/platform_dependent/windows_X.hpp>
#include <tetengo/text/encoder.hpp>
#include <tetengo/text/encoding/utf16.hpp>


namespace tetengo::platform_dependent
{
    class windows_registry_reader::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(const std::filesystem::path& subkey) :
        m_native_subkey{ to_native_subkey(subkey) },
            m_handle{ open_registry_key(m_native_subkey) }
        {}

        ~impl()
        {
            ::RegCloseKey(m_handle);
        }


        // functions

        std::optional<std::uint32_t> dword_value_of(const std::string_view& name) const
        {
            auto       type = static_cast<::DWORD>(0);
            auto       data = static_cast<::DWORD>(0);
            auto       data_length = static_cast<::DWORD>(sizeof(::DWORD));
            const auto result = ::RegGetValueW(
                HKEY_CURRENT_USER,
                m_native_subkey.c_str(),
                to_wstring(encoder().encode(name)).c_str(),
                RRF_RT_REG_DWORD,
                &type,
                &data,
                &data_length);
            if (result != ERROR_SUCCESS)
            {
                return std::nullopt;
            }
            assert(type == REG_DWORD);
            return std::make_optional(data);
        }

        std::optional<std::string> string_value_of(const std::string_view& name) const
        {
            auto data_length_in_bytes = static_cast<::DWORD>(0);
            {
                auto       type = static_cast<::DWORD>(0);
                const auto result = ::RegGetValueW(
                    HKEY_CURRENT_USER,
                    m_native_subkey.c_str(),
                    to_wstring(encoder().encode(name)).c_str(),
                    RRF_RT_REG_SZ,
                    &type,
                    nullptr,
                    &data_length_in_bytes);
                if (result != ERROR_SUCCESS)
                {
                    return std::nullopt;
                }
                assert(type == REG_SZ);
            }
            std::vector<wchar_t> data(data_length_in_bytes / sizeof(wchar_t), L'\0');
            {
                auto       type = static_cast<::DWORD>(0);
                const auto result = ::RegGetValueW(
                    HKEY_CURRENT_USER,
                    m_native_subkey.c_str(),
                    to_wstring(encoder().encode(name)).c_str(),
                    RRF_RT_REG_SZ,
                    &type,
                    std::data(data),
                    &data_length_in_bytes);
                if (result != ERROR_SUCCESS)
                {
                    return std::nullopt;
                }
                assert(type == REG_SZ);
            }
            assert(data_length_in_bytes / sizeof(wchar_t) > 0);
            const std::wstring_view value{ std::data(data), data_length_in_bytes / sizeof(wchar_t) - 1 };
            return std::make_optional(encoder().decode(to_u16string(value)));
        }


    private:
        // static functions

        static std::filesystem::path to_native_subkey(const std::filesystem::path& subkey)
        {
            return "SOFTWARE" / subkey;
        }

        static ::HKEY open_registry_key(const std::filesystem::path& native_subkey)
        {
            ::HKEY     handle = nullptr;
            const auto result = ::RegOpenKeyExW(HKEY_CURRENT_USER, native_subkey.c_str(), 0, KEY_READ, &handle);
            if (result != ERROR_SUCCESS)
            {
                throw std::runtime_error{ "Can't open the Windows registry key." };
            }
            return handle;
        }

        static const tetengo::text::encoder<tetengo::text::encoding::utf16>& encoder()
        {
            static const tetengo::text::encoder<tetengo::text::encoding::utf16> singleton;
            return singleton;
        }

        static const std::wstring to_wstring(const std::u16string_view& utf16)
        {
            static_assert(sizeof(wchar_t) == sizeof(char16_t));
            static_assert(static_cast<std::uint16_t>(L'\u12AB') == static_cast<std::uint16_t>(u'\u12AB'));
            return std::wstring{ std::begin(utf16), std::end(utf16) };
        }

        static const std::u16string to_u16string(const std::wstring_view& wide)
        {
            static_assert(sizeof(wchar_t) == sizeof(char16_t));
            static_assert(static_cast<std::uint16_t>(L'\u12AB') == static_cast<std::uint16_t>(u'\u12AB'));
            return std::u16string{ std::begin(wide), std::end(wide) };
        }


        // variables

        const std::filesystem::path m_native_subkey;

        const ::HKEY m_handle;
    };


    windows_registry_reader::windows_registry_reader(const std::filesystem::path& subkey) :
    m_p_impl{ std::make_unique<impl>(subkey) }
    {}

    windows_registry_reader::~windows_registry_reader() = default;

    std::optional<std::uint32_t> windows_registry_reader::dword_value_of(const std::string_view& name) const
    {
        return m_p_impl->dword_value_of(name);
    }

    std::optional<std::string> windows_registry_reader::string_value_of(const std::string_view& name) const
    {
        return m_p_impl->string_value_of(name);
    }


}
