/*! \file
    \brief A text encoder.

    For Windows.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#define NOMINMAX
#include <Windows.h>

#include <boost/core/noncopyable.hpp>

#include <tetengo/platform_dependent/text_encoder.hpp>


namespace tetengo::platform_dependent
{
    class text_encoder::impl : private boost::noncopyable
    {
    public:
        // static functions

        static const text_encoder& instance()
        {
            static const text_encoder singleton{};
            return singleton;
        }


        // functions

        std::string encode_to_cp932(const std::string_view& utf8) const
        {
            return to_multibyte(to_wide(utf8, CP_UTF8), 932);
        }

        std::string decode_from_cp932(const std::string_view& cp932) const
        {
            return std::string{ cp932 };
        }


    private:
        // static functions

        static std::wstring to_wide(const std::string_view& multibyte, const ::UINT code_page)
        {
            const auto wide_length = ::MultiByteToWideChar(
                code_page, 0, std::data(multibyte), static_cast<int>(multibyte.length()), nullptr, 0);
            if (wide_length == 0)
            {
                throw std::invalid_argument{ "Invalid multibyte string." };
            }

            std::vector<wchar_t> wide(wide_length, 0);
            const auto           wide_length_again = ::MultiByteToWideChar(
                code_page, 0, std::data(multibyte), static_cast<int>(multibyte.length()), std::data(wide), wide_length);
            assert(wide_length_again == wide_length);

            return std::wstring{ std::begin(wide), std::end(wide) };
        }

        static std::string to_multibyte(const std::wstring_view& wide, const ::UINT code_page)
        {
            constexpr const char* replacement_for_inconvertible = "?";

            const auto multibyte_length = ::WideCharToMultiByte(
                code_page,
                code_page == CP_UTF8 ? 0 : WC_NO_BEST_FIT_CHARS,
                std::data(wide),
                static_cast<int>(wide.length()),
                nullptr,
                0,
                replacement_for_inconvertible,
                nullptr);
            if (multibyte_length == 0)
            {
                throw std::invalid_argument{ "Invalid wide string." };
            }

            std::vector<char> multibyte(multibyte_length, 0);
            const auto        multibyte_length_again = ::WideCharToMultiByte(
                code_page,
                code_page == CP_UTF8 ? 0 : WC_NO_BEST_FIT_CHARS,
                std::data(wide),
                static_cast<int>(wide.length()),
                std::data(multibyte),
                multibyte_length,
                replacement_for_inconvertible,
                nullptr);
            assert(multibyte_length_again == multibyte_length);

            return std::string{ std::begin(multibyte), std::end(multibyte) };
        }
    };


    const text_encoder& text_encoder::instance()
    {
        return impl::instance();
    }

    text_encoder::~text_encoder() = default;

    std::string text_encoder::encode_to_cp932(const std::string_view& utf8) const
    {
        return m_p_impl->encode_to_cp932(utf8);
    }

    std::string text_encoder::decode_from_cp932(const std::string_view& cp932) const
    {
        return m_p_impl->decode_from_cp932(cp932);
    }

    text_encoder::text_encoder() : m_p_impl{ std::make_unique<impl>() } {}
}
