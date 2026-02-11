/*! \file
    \brief An install bootstrapper.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <clocale>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include <Windows.h>

#include <tetengo/text/encoder.hpp>
#include <tetengo/text/encoding/utf16.hpp>


namespace
{
    std::string locale_name()
    {
        const auto* const name = ::setlocale(LC_ALL, "");
        return name ? name : "";
    }

    std::filesystem::path base_directory()
    {
        std::vector<wchar_t> path_string(MAX_PATH, 0);
        ::GetModuleFileNameW(nullptr, std::data(path_string), static_cast<::DWORD>(std::size(path_string)));
        return std::filesystem::path(std::data(path_string)).parent_path();
    }

    std::wstring to_utf16(const std::string_view& utf8)
    {
        static const tetengo::text::encoder<tetengo::text::encoding::utf16> encoder{};
        return reinterpret_cast<const wchar_t*>(encoder.encode(utf8).c_str());
    }

    std::filesystem::path load_conf(const std::filesystem::path& base_directory)
    {
        const auto    conf_path = base_directory / L"setup.conf";
        std::ifstream stream{ conf_path };
        if (!stream)
        {
            throw std::runtime_error{ "Can't open setup.conf." };
        }
        std::string msi_file_name{};
        std::getline(stream, msi_file_name);
        return to_utf16(msi_file_name);
    }

    void execute_installer(
        const std::filesystem::path& base_directory,
        const std::filesystem::path& msi_file_name,
        const std::string_view&      locale_name,
        const int                    show_command)
    {
        const auto msi_path = base_directory / msi_file_name;
        auto       parameters = std::wstring{ L"/i \"" } + msi_path.native() + std::wstring{ L"\"" };
        if (locale_name.starts_with("Japanese"))
        {
            parameters += L" TRANSFORMS=\":ja.mst\"";
        }
        ::ShellExecuteW(NULL, NULL, L"msiexec", parameters.c_str(), base_directory.c_str(), show_command);
    }


}


int WINAPI WinMain(
    const ::HINSTANCE /*hInstance*/,
    const ::HINSTANCE /*hPrevInstance*/,
    const ::LPSTR /*lpCmdLine*/,
    const int nShowCmd)
{
    try
    {
        const auto locale_name_ = locale_name();
        const auto base_directory_ = base_directory();
        const auto msi_file_name = load_conf(base_directory_);
        execute_installer(base_directory_, msi_file_name, locale_name_, nShowCmd);
        return 0;
    }
    catch (const std::exception& e)
    {
        ::MessageBoxW(NULL, to_utf16(e.what()).c_str(), L"Setup Error", MB_ICONERROR | MB_OK);
        return 1;
    }
    catch (...)
    {
        ::MessageBoxW(NULL, L"Unknown error.", L"Setup Error", MB_ICONERROR | MB_OK);
        return 1;
    }
}
