/*! \file
    \brief A terminal string width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <iterator>
#include <locale>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/cli/character_width.hpp>
#include <tetengo/cli/default_character_width.hpp>
#include <tetengo/cli/terminal_string_width.hpp>


namespace tetengo::cli
{
    class terminal_string_width::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(const std::locale& locale_) : m_character_width{ select_character_width(locale_) } {}


        // functions

        std::size_t width_of(const std::string_view& string_) const
        {
            const auto code_points = to_code_points(string_);

            auto width = static_cast<std::size_t>(0);
            auto previous = std::end(code_points);
            for (auto i = std::begin(code_points); i != std::end(code_points); ++i)
            {
                width += m_character_width.width_of(previous == std::end(code_points) ? 0 : *previous, *i);
                previous = i;
            }

            return width;
        }


    private:
        // static functions

        static const character_width& select_character_width(const std::locale& /*locale_*/)
        {
            return default_character_width::instance();
        }

        static std::vector<char32_t> to_code_points(const std::string_view& string)
        {
            std::vector<char32_t> code_points{};

            auto       current = std::begin(string);
            const auto last = std::end(string);
            while (current != last)
            {
                char32_t code_point = 0;
                if (const auto leading = static_cast<unsigned char>(*current); leading <= 0x7F)
                {
                    ++current;
                    code_point = leading;
                }
                else if (0xC0 <= leading && leading <= 0xDF)
                {
                    char32_t value = (leading & 0x1F) << 6;
                    ++current;
                    value |= following_value(current, last, 1);
                    if (value <= 0x0000007F)
                    {
                        throw std::invalid_argument{ "The string is not in valid UTF-8." };
                    }
                    code_point = value;
                }
                else if (0xE0 <= leading && leading <= 0xEF)
                {
                    char32_t value = (leading & 0x0F) << 12;
                    ++current;
                    value |= following_value(current, last, 2);
                    if (value <= 0x000007FF)
                    {
                        throw std::invalid_argument{ "The string is not in valid UTF-8." };
                    }
                    code_point = value;
                }
                else if (0xF0 <= leading && leading <= 0xF7)
                {
                    char32_t value = (leading & 0x07) << 18;
                    ++current;
                    value |= following_value(current, last, 3);
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

                code_points.push_back(code_point);
            }

            return code_points;
        }

        static char32_t following_value(
            std::string_view::const_iterator&      current,
            const std::string_view::const_iterator last,
            const std::size_t                      following_count)
        {
            char32_t value = 0;
            for (auto i = static_cast<std::size_t>(0); i < following_count; ++i)
            {
                const auto following = following_byte(current, last);
                value |= following << (6 * (following_count - i - 1));
                ++current;
            }
            return value;
        }

        static unsigned char
        following_byte(const std::string_view::const_iterator position, const std::string_view::const_iterator last)
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


        // variables

        const character_width& m_character_width;
    };


    terminal_string_width::terminal_string_width(const std::locale& locale_ /*= std::locale{ "" }*/) :
    m_p_impl{ std::make_unique<impl>(locale_) }
    {}

    terminal_string_width::~terminal_string_width() = default;

    std::size_t terminal_string_width::width_of(const std::string_view& string_) const
    {
        return m_p_impl->width_of(string_);
    }


}
