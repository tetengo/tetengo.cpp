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
            std::pair<std::u32string, std::vector<std::size_t>> code_points_and_offsets{};

            auto       current = std::begin(utf8);
            const auto last = std::end(utf8);
            while (current != last)
            {
                auto       code_point = static_cast<char32_t>(0);
                const auto offset = static_cast<std::size_t>(std::distance(std::begin(utf8), current));
                if (const auto leading = static_cast<unsigned char>(*current); leading <= 0x7F)
                {
                    ++current;
                    code_point = leading;
                }
                else if (0xC0 <= leading && leading <= 0xDF)
                {
                    auto value = static_cast<char32_t>((leading & 0x1F) << 6);
                    ++current;
                    value |= utf8_following_value(current, last, 1);
                    if (value <= 0x0000007F)
                    {
                        throw std::invalid_argument{ "The string is not in valid UTF-8." };
                    }
                    code_point = value;
                }
                else if (0xE0 <= leading && leading <= 0xEF)
                {
                    auto value = static_cast<char32_t>((leading & 0x0F) << 12);
                    ++current;
                    value |= utf8_following_value(current, last, 2);
                    if (value <= 0x000007FF)
                    {
                        throw std::invalid_argument{ "The string is not in valid UTF-8." };
                    }
                    code_point = value;
                }
                else if (0xF0 <= leading && leading <= 0xF7)
                {
                    auto value = static_cast<char32_t>((leading & 0x07) << 18);
                    ++current;
                    value |= utf8_following_value(current, last, 3);
                    if (value <= 0x0000FFFF)
                    {
                        throw std::invalid_argument{ "The string is not in valid UTF-8." };
                    }
                    code_point = value;
                }
                else
                {
                    throw std::invalid_argument{ "The string is not in valid UTF-8." };
                }

                code_points_and_offsets.first.push_back(code_point);
                code_points_and_offsets.second.push_back(offset);
            }

            return code_points_and_offsets;
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

    unicode_encoding::unicode_encoding() : m_p_impl{ std::make_unique<impl>() } {}


}
