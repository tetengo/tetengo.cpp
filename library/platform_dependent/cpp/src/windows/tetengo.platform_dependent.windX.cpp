/*! \file
    \brief A Windows registry access.

    For Windows.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <Windows.h>

#include <boost/core/noncopyable.hpp>

#include <tetengo/platform_dependent/windows_X.hpp>
#include <tetengo/text/encoder.hpp>
#include <tetengo/text/encoding/utf16.hpp>


namespace tetengo::platform_dependent
{
    namespace
    {
        std::filesystem::path to_native_subkey(const std::filesystem::path& subkey)
        {
            return "SOFTWARE" / subkey;
        }

        const tetengo::text::encoder<tetengo::text::encoding::utf16>& encoder()
        {
            static const tetengo::text::encoder<tetengo::text::encoding::utf16> singleton;
            return singleton;
        }

        const std::wstring to_wstring(const std::u16string_view& utf16)
        {
            static_assert(sizeof(wchar_t) == sizeof(char16_t));
            static_assert(static_cast<std::uint16_t>(L'\u12AB') == static_cast<std::uint16_t>(u'\u12AB'));
            return std::wstring{ std::begin(utf16), std::end(utf16) };
        }

        const std::u16string to_u16string(const std::wstring_view& wide)
        {
            static_assert(sizeof(wchar_t) == sizeof(char16_t));
            static_assert(static_cast<std::uint16_t>(L'\u12AB') == static_cast<std::uint16_t>(u'\u12AB'));
            return std::u16string{ std::begin(wide), std::end(wide) };
        }


    }


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

        std::vector<std::string> value_names() const
        {
            std::vector<std::string> names{};
            for (auto i = static_cast<::DWORD>(0);; ++i)
            {
                std::vector<wchar_t> wstring_name(32768, L'\0');
                auto                 name_length = static_cast<::DWORD>(std::size(wstring_name));
                const auto           result =
                    ::RegEnumValueW(m_handle, i, std::data(wstring_name), &name_length, NULL, NULL, NULL, NULL);
                if (result != ERROR_SUCCESS)
                {
                    break;
                }
                names.push_back(
                    encoder().decode(to_u16string(std::wstring_view{ std::data(wstring_name), name_length })));
            }
            return names;
        }

        std::optional<std::uint32_t> dword_value_of(const std::string_view& name) const
        {
            auto       type = static_cast<::DWORD>(0);
            auto       data = static_cast<::DWORD>(0);
            auto       data_length = static_cast<::DWORD>(sizeof(::DWORD));
            const auto result = ::RegQueryValueExW(
                m_handle,
                to_wstring(encoder().encode(name)).c_str(),
                nullptr,
                &type,
                reinterpret_cast<::BYTE*>(&data),
                &data_length);
            if (result != ERROR_SUCCESS || type != REG_DWORD)
            {
                return std::nullopt;
            }
            return std::make_optional(data);
        }

        std::optional<std::string> string_value_of(const std::string_view& name) const
        {
            auto data_length_in_bytes = static_cast<::DWORD>(0);
            {
                auto       type = static_cast<::DWORD>(0);
                const auto result = ::RegQueryValueExW(
                    m_handle,
                    to_wstring(encoder().encode(name)).c_str(),
                    nullptr,
                    &type,
                    nullptr,
                    &data_length_in_bytes);
                if (result != ERROR_SUCCESS || type != REG_SZ)
                {
                    return std::nullopt;
                }
            }
            std::vector<wchar_t> data(data_length_in_bytes / sizeof(wchar_t), L'\0');
            {
                auto       type = static_cast<::DWORD>(0);
                const auto result = ::RegQueryValueExW(
                    m_handle,
                    to_wstring(encoder().encode(name)).c_str(),
                    nullptr,
                    &type,
                    reinterpret_cast<::BYTE*>(std::data(data)),
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

        static ::HKEY open_registry_key(const std::filesystem::path& subkey)
        {
            ::HKEY     handle = nullptr;
            const auto result =
                ::RegOpenKeyExW(HKEY_CURRENT_USER, to_native_subkey(subkey).c_str(), 0, KEY_READ, &handle);
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

    std::vector<std::string> windows_registry_reader::value_names() const
    {
        return m_p_impl->value_names();
    }

    std::optional<std::uint32_t> windows_registry_reader::dword_value_of(const std::string_view& name) const
    {
        return m_p_impl->dword_value_of(name);
    }

    std::optional<std::string> windows_registry_reader::string_value_of(const std::string_view& name) const
    {
        return m_p_impl->string_value_of(name);
    }


    class windows_registry_writer::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(const std::filesystem::path& subkey) : m_handle{ create_registry_key(subkey) } {}

        ~impl()
        {
            ::RegCloseKey(m_handle);
        }


        // functions

        void set_dword_value_of(const std::string_view& name, const std::uint32_t value) const
        {
            const auto dword_value = static_cast<::DWORD>(value);
            const auto result = ::RegSetValueExW(
                m_handle,
                to_wstring(encoder().encode(name)).c_str(),
                0,
                REG_DWORD,
                reinterpret_cast<const ::BYTE*>(&dword_value),
                sizeof(::DWORD));
            if (result != ERROR_SUCCESS)
            {
                throw std::runtime_error{ "Can't set a DWORD value to the registry." };
            }
        }

        void set_string_value_of(const std::string_view& name, const std::string_view& value) const
        {
            const auto wstring_value = to_wstring(encoder().encode(value));
            const auto result = ::RegSetValueExW(
                m_handle,
                to_wstring(encoder().encode(name)).c_str(),
                0,
                REG_SZ,
                reinterpret_cast<const ::BYTE*>(wstring_value.c_str()),
                static_cast<::DWORD>((wstring_value.length() + 1) * sizeof(wchar_t)));
            if (result != ERROR_SUCCESS)
            {
                throw std::runtime_error{ "Can't set a string value to the registry." };
            }
        }


    private:
        // static functions

        static ::HKEY create_registry_key(const std::filesystem::path& subkey)
        {
            ::HKEY     handle = nullptr;
            auto       disposition = static_cast<::DWORD>(0);
            const auto result = ::RegCreateKeyExW(
                HKEY_CURRENT_USER,
                to_native_subkey(subkey).c_str(),
                0,
                nullptr,
                REG_OPTION_NON_VOLATILE,
                KEY_WRITE,
                nullptr,
                &handle,
                &disposition);
            if (result != ERROR_SUCCESS)
            {
                throw std::runtime_error{ "Can't create as Windows registry key." };
            }
            return handle;
        }


        // variables

        const ::HKEY m_handle;
    };


    windows_registry_writer::windows_registry_writer(const std::filesystem::path& subkey) :
    m_p_impl{ std::make_unique<impl>(subkey) }
    {}

    windows_registry_writer::~windows_registry_writer() = default;

    void windows_registry_writer::set_dword_value_of(const std::string_view& name, const std::uint32_t value) const
    {
        return m_p_impl->set_dword_value_of(name, value);
    }

    void windows_registry_writer::set_string_value_of(const std::string_view& name, const std::string_view& value) const
    {
        return m_p_impl->set_string_value_of(name, value);
    }


}
