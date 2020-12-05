/*! \file
    \brief A unicode encoding.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <iterator>
#include <memory>
#include <stdexcept>

#include <boost/core/noncopyable.hpp>

#include "tetengo.text.encoding.unicode_encoding.hpp"


namespace tetengo::text::encoding
{
    class unicode_encoding::impl : private boost::noncopyable
    {
    public:
        // static functions

        static const unicode_encoding& instance()
        {
            static const unicode_encoding singleton{};
            return singleton;
        }


        // functions

        std::pair<std::u32string, std::vector<std::size_t>> utf8_to_codepoints(const std::string_view& utf8) const
        {
            std::pair<std::u32string, std::vector<std::size_t>> codepoints_and_offsets{};

            auto       current = std::begin(utf8);
            const auto last = std::end(utf8);
            while (current != last)
            {
                constexpr auto inconvertible = '?';
                const auto     offset = static_cast<std::size_t>(std::distance(std::begin(utf8), current));
                if (const auto leading = static_cast<unsigned char>(*current); leading <= 0x7F)
                {
                    ++current;
                    codepoints_and_offsets.first.push_back(leading);
                    codepoints_and_offsets.second.push_back(offset);
                }
                else if (0xC0 <= leading && leading <= 0xDF)
                {
                    auto value = static_cast<char32_t>((leading & 0x1F) << 6);
                    ++current;
                    value |= utf8_following_value(current, last, 1);
                    if (value >= 0x00000080)
                    {
                        codepoints_and_offsets.first.push_back(value);
                        codepoints_and_offsets.second.push_back(offset);
                    }
                    else
                    {
                        for (auto i = static_cast<std::size_t>(0); i < 2; ++i)
                        {
                            codepoints_and_offsets.first.push_back(inconvertible);
                            codepoints_and_offsets.second.push_back(offset + i);
                        }
                    }
                }
                else if (0xE0 <= leading && leading <= 0xEF)
                {
                    auto value = static_cast<char32_t>((leading & 0x0F) << 12);
                    ++current;
                    value |= utf8_following_value(current, last, 2);
                    if (value >= 0x00000800)
                    {
                        codepoints_and_offsets.first.push_back(value);
                        codepoints_and_offsets.second.push_back(offset);
                    }
                    else
                    {
                        for (auto i = static_cast<std::size_t>(0); i < 3; ++i)
                        {
                            codepoints_and_offsets.first.push_back(inconvertible);
                            codepoints_and_offsets.second.push_back(offset + i);
                        }
                    }
                }
                else if (0xF0 <= leading && leading <= 0xF7)
                {
                    auto value = static_cast<char32_t>((leading & 0x07) << 18);
                    ++current;
                    value |= utf8_following_value(current, last, 3);
                    if (value >= 0x00010000)
                    {
                        codepoints_and_offsets.first.push_back(value);
                        codepoints_and_offsets.second.push_back(offset);
                    }
                    else
                    {
                        for (auto i = static_cast<std::size_t>(0); i < 4; ++i)
                        {
                            codepoints_and_offsets.first.push_back(inconvertible);
                            codepoints_and_offsets.second.push_back(offset + i);
                        }
                    }
                }
                else
                {
                    codepoints_and_offsets.first.push_back(inconvertible);
                    codepoints_and_offsets.second.push_back(offset);
                }
            }

            return codepoints_and_offsets;
        }

        std::string codepoints_to_utf8(const std::u32string_view& codepoints) const
        {
            std::string utf8;
            for (const auto codepoint: codepoints)
            {
                if (codepoint < 0x00000080)
                {
                    utf8.push_back(static_cast<char>(codepoint));
                }
                else if (codepoint < 0x00000800)
                {
                    const auto leading = static_cast<unsigned char>(0xC0 | (codepoint >> 6));
                    utf8.push_back(leading);
                    utf8.push_back(utf8_following_code_unit(codepoint, 1));
                }
                else if (codepoint < 0x00010000)
                {
                    const auto leading = static_cast<unsigned char>(0xE0 | (codepoint >> 12));
                    utf8.push_back(leading);
                    utf8.push_back(utf8_following_code_unit(codepoint, 2));
                    utf8.push_back(utf8_following_code_unit(codepoint, 1));
                }
                else if (codepoint < 0x00020000)
                {
                    const auto leading = static_cast<unsigned char>(0xF0 | (codepoint >> 18));
                    utf8.push_back(leading);
                    utf8.push_back(utf8_following_code_unit(codepoint, 3));
                    utf8.push_back(utf8_following_code_unit(codepoint, 2));
                    utf8.push_back(utf8_following_code_unit(codepoint, 1));
                }
                else
                {
                    constexpr auto inconvertible = '?';
                    utf8.push_back(inconvertible);
                }
            }
            return utf8;
        }

        std::pair<std::u32string, std::vector<std::size_t>> utf16_to_codepoints(const std::u16string_view& utf16) const
        {
            std::pair<std::u32string, std::vector<std::size_t>> codepoints_and_offsets{};
            codepoints_and_offsets.first.reserve(utf16.length());
            codepoints_and_offsets.second.reserve(utf16.length());

            auto       current = std::begin(utf16);
            const auto last = std::end(utf16);
            while (current != last)
            {
                constexpr auto inconvertible = U'?';

                const auto offset = static_cast<std::size_t>(std::distance(std::begin(utf16), current));
                if (const auto code_unit = *current; code_unit < 0xD800 || 0xDFFF < code_unit)
                {
                    codepoints_and_offsets.first.push_back(code_unit);
                    codepoints_and_offsets.second.push_back(offset);
                    ++current;
                }
                else if (0xD800 <= code_unit && code_unit <= 0xDBFF && std::next(current) != last)
                {
                    if (const auto next_code_unit = *std::next(current);
                        0xDC00 <= next_code_unit && next_code_unit <= 0xDFFF)
                    {
                        const auto codepoint = (code_unit - 0xD800) * 1024 + (next_code_unit - 0xDC00) + 0x10000;
                        codepoints_and_offsets.first.push_back(codepoint);
                        codepoints_and_offsets.second.push_back(offset);
                    }
                    else
                    {
                        codepoints_and_offsets.first.push_back(inconvertible);
                        codepoints_and_offsets.second.push_back(offset);
                        if (next_code_unit < 0xD800 || 0xDFFF < next_code_unit)
                        {
                            codepoints_and_offsets.first.push_back(code_unit);
                        }
                        else
                        {
                            codepoints_and_offsets.first.push_back(inconvertible);
                        }
                        codepoints_and_offsets.second.push_back(offset + 1);
                    }
                    current += 2;
                }
                else
                {
                    codepoints_and_offsets.first.push_back(inconvertible);
                    codepoints_and_offsets.second.push_back(offset);
                    ++current;
                }
            }

            return codepoints_and_offsets;
        }

        std::u16string codepoints_to_utf16(const std::u32string_view& codepoints) const
        {
            std::u16string utf16;
            utf16.reserve(codepoints.length());
            for (const auto codepoint: codepoints)
            {
                if (codepoint < 0x00010000)
                {
                    utf16.push_back(static_cast<char16_t>(codepoint));
                }
                else if (codepoint < 0x00020000)
                {
                    const auto high_surrogate = static_cast<char16_t>(0xD800 + (codepoint - 0x10000) / 0x400);
                    utf16.push_back(high_surrogate);
                    const auto low_surrogate = static_cast<char16_t>(0xDC00 + (codepoint - 0x10000) % 0x400);
                    utf16.push_back(low_surrogate);
                }
                else
                {
                    constexpr auto inconvertible = u'?';
                    utf16.push_back(inconvertible);
                }
            }
            return utf16;
        }


    private:
        // static functions

        static char32_t utf8_following_value(
            std::string_view::const_iterator&      current,
            const std::string_view::const_iterator last,
            const std::size_t                      following_count)
        {
            char32_t value = 0;
            for (auto i = static_cast<std::size_t>(0); i < following_count; ++i)
            {
                const auto following = utf8_following_byte(current, last);
                value |= following << (6 * (following_count - i - 1));
                ++current;
            }
            return value;
        }

        static unsigned char utf8_following_byte(
            const std::string_view::const_iterator position,
            const std::string_view::const_iterator last)
        {
            if (position == last)
            {
                throw std::invalid_argument{ "The string is not in valid UTF-8." };
            }

            const auto following = static_cast<unsigned char>(*position);
            if (following < 0x80 || 0xBF < following)
            {
                throw std::invalid_argument{ "The string is not in valid UTF-8." };
            }

            return following & 0x3F;
        }

        static unsigned char utf8_following_code_unit(const char32_t codepoint, const std::size_t following_index)
        {
            return static_cast<unsigned char>(0x80 | ((codepoint >> (6 * (following_index - 1))) & 0x3F));
        }
    };


    const unicode_encoding& unicode_encoding::instance()
    {
        return impl::instance();
    }

    unicode_encoding::~unicode_encoding() = default;

    std::pair<std::u32string, std::vector<std::size_t>>
    unicode_encoding::utf8_to_codepoints(const std::string_view& utf8) const
    {
        return m_p_impl->utf8_to_codepoints(utf8);
    }

    std::string unicode_encoding::codepoints_to_utf8(const std::u32string_view& codepoints) const
    {
        return m_p_impl->codepoints_to_utf8(codepoints);
    }

    std::pair<std::u32string, std::vector<std::size_t>>
    unicode_encoding::utf16_to_codepoints(const std::u16string_view& utf16) const
    {
        return m_p_impl->utf16_to_codepoints(utf16);
    }

    std::u16string unicode_encoding::codepoints_to_utf16(const std::u32string_view& codepoints) const
    {
        return m_p_impl->codepoints_to_utf16(codepoints);
    }

    unicode_encoding::unicode_encoding() : m_p_impl{ std::make_unique<impl>() } {}
}
