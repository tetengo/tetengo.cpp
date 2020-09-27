/*! \file
    \brief A terminal string width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cassert>
#include <cctype>
#include <clocale>
#include <cstddef>
#include <iterator>
#include <locale>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/cli/character_width.hpp>
#include <tetengo/cli/default_character_width.hpp>
#include <tetengo/cli/east_asian_character_width.hpp>
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

            std::vector<std::size_t> widths{};
            widths.reserve(code_points.size());
            std::transform(
                std::begin(code_points),
                std::end(code_points),
                std::back_inserter(widths),
                [this](const auto code_point) { return m_character_width.width_of(code_point); });

            return std::accumulate(std::begin(widths), std::end(widths), static_cast<std::size_t>(0));
        }


    private:
        // static functions

        static const character_width& select_character_width(const std::locale& locale_)
        {
            if (is_east_asian_locale(locale_))
            {
                return east_asian_character_width::instance();
            }
            else
            {
                return default_character_width::instance();
            }
        }

        static bool is_east_asian_locale(const std::locale& locale_)
        {
            auto name = locale_name(locale_);
            std::transform(std::begin(name), std::end(name), std::begin(name), [](const char c) {
                return static_cast<char>(std::tolower(c));
            });
            const std::string_view name_view{ name.c_str() };

#if _WIN32
            constexpr const char* const separator = "_-";
#else
            constexpr const char* const separator = "_";
#endif
            const std::string_view language = name_view.substr(0, name.find_first_of(separator, 0));

            if (language == "zh" || language == "ja" || language == "ko")
            {
                return true;
            }
#if _WIN32
            else if (
                language == "chinese" || language == "chinese (simplified)" || language == "chinese (traditional)" ||
                language == "japanese" || language == "korean")
            {
                return true;
            }
            else if (language == "chs" || language == "cht" || language == "jpn" || language == "kor")
            {
                return true;
            }
#endif

            return false;
        }

        static std::string locale_name(const std::locale& locale_)
        {
            auto cpp_name = locale_.name();
            if (!cpp_name.empty())
            {
                return cpp_name;
            }

            const auto* const c_name = std::setlocale(LC_CTYPE, nullptr);
            assert(c_name);
            return c_name;
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
