/*! \file
    \brief A grapheme splitter.

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
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/text/grapheme_splitter.hpp>
#include <tetengo/text/grapheme_splitting/character_width.hpp>
#include <tetengo/text/grapheme_splitting/default_character_width.hpp>
#include <tetengo/text/grapheme_splitting/east_asian_character_width.hpp>
#include <tetengo/text/grapheme_splitting/grapheme_segment.hpp>

#include "tetengo.text.grapheme_splitting.character_property_map.hpp"


namespace tetengo::text
{
    class grapheme_splitter::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(const std::locale& locale_) : m_character_width{ select_character_width(locale_) } {}


        // functions

        std::vector<grapheme> split(const std::string_view& string_) const
        {
            const auto code_points_and_offsets = to_code_points(string_);

            std::vector<std::size_t> widths{};
            widths.reserve(std::size(code_points_and_offsets));
            std::vector<grapheme_splitting::grapheme_segment::break_property_type> break_properties{};
            break_properties.reserve(std::size(code_points_and_offsets));
            for (const auto code_point_and_offset: code_points_and_offsets)
            {
                const auto property = property_of(code_point_and_offset.first);
                widths.push_back(m_character_width.width_of(std::get<0>(property), std::get<1>(property)));
                break_properties.push_back(std::get<2>(property));
            }
            const auto grapheme_offsets =
                grapheme_splitting::grapheme_segment::instance().segment_offsets(break_properties);

            std::vector<grapheme> graphemes{};
            graphemes.reserve(std::size(grapheme_offsets));
            for (auto i = static_cast<std::size_t>(0); i + 1 < std::size(grapheme_offsets); ++i)
            {
                graphemes.emplace_back(
                    code_points_and_offsets[grapheme_offsets[i]].second,
                    *std::max_element(
                        std::next(std::begin(widths), grapheme_offsets[i]),
                        std::next(std::begin(widths), grapheme_offsets[i + 1])));
            }

            return graphemes;
        }


    private:
        // static functions

        static const grapheme_splitting::character_width& select_character_width(const std::locale& locale_)
        {
            if (is_east_asian_locale(locale_))
            {
                return grapheme_splitting::east_asian_character_width::instance();
            }
            else
            {
                return grapheme_splitting::default_character_width::instance();
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
            if (!std::empty(cpp_name))
            {
                return cpp_name;
            }

            const auto* const c_name = std::setlocale(LC_CTYPE, nullptr);
            assert(c_name);
            return c_name;
        }

        static std::vector<std::pair<char32_t, std::size_t>> to_code_points(const std::string_view& string)
        {
            std::vector<std::pair<char32_t, std::size_t>> code_points_and_offsets{};

            auto       current = std::begin(string);
            const auto last = std::end(string);
            while (current != last)
            {
                auto       code_point = static_cast<char32_t>(0);
                const auto offset = static_cast<std::size_t>(std::distance(std::begin(string), current));
                if (const auto leading = static_cast<unsigned char>(*current); leading <= 0x7F)
                {
                    ++current;
                    code_point = leading;
                }
                else if (0xC0 <= leading && leading <= 0xDF)
                {
                    auto value = static_cast<char32_t>((leading & 0x1F) << 6);
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
                    auto value = static_cast<char32_t>((leading & 0x0F) << 12);
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
                    auto value = static_cast<char32_t>((leading & 0x07) << 18);
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

                code_points_and_offsets.push_back(std::make_pair(code_point, offset));
            }

            return code_points_and_offsets;
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

        static std::tuple<
            grapheme_splitting::character_width::class_type,
            grapheme_splitting::character_width::emoji_type,
            grapheme_splitting::grapheme_segment::break_property_type>
        property_of(const char32_t code_point)
        {
            const auto* p_lower_bound = std::lower_bound(
                grapheme_splitting::character_property_map,
                grapheme_splitting::character_property_map + grapheme_splitting::character_property_map_size,
                grapheme_splitting::character_property_map_element_type{
                    code_point,
                    grapheme_splitting::east_asian_width_class_type::ambiguous,
                    grapheme_splitting::emoji_type::normal,
                    grapheme_splitting::grapheme_break_property_type::other },
                [](const auto& element1, const auto& element2) { return element1.code_point < element2.code_point; });

            if (p_lower_bound !=
                    grapheme_splitting::character_property_map + grapheme_splitting::character_property_map_size &&
                p_lower_bound->code_point == code_point)
            {
                return std::make_tuple(
                    to_class_type(p_lower_bound->class_),
                    to_emoji_type(p_lower_bound->emoji),
                    to_break_property(p_lower_bound->grapheme));
            }

            assert(p_lower_bound != grapheme_splitting::character_property_map);
            const auto* const p_previous = std::prev(p_lower_bound);
            return std::make_tuple(
                to_class_type(p_previous->class_),
                to_emoji_type(p_previous->emoji),
                to_break_property(p_previous->grapheme));
        }

        static grapheme_splitting::character_width::class_type
        to_class_type(const grapheme_splitting::east_asian_width_class_type class_type)
        {
            switch (class_type)
            {
            case grapheme_splitting::east_asian_width_class_type::ambiguous:
                return grapheme_splitting::character_width::class_type::ambiguous;
            case grapheme_splitting::east_asian_width_class_type::fullwidth:
                return grapheme_splitting::character_width::class_type::fullwidth;
            case grapheme_splitting::east_asian_width_class_type::halfwidth:
                return grapheme_splitting::character_width::class_type::halfwidth;
            case grapheme_splitting::east_asian_width_class_type::narrow:
                return grapheme_splitting::character_width::class_type::narrow;
            case grapheme_splitting::east_asian_width_class_type::wide:
                return grapheme_splitting::character_width::class_type::wide;
            default:
                assert(class_type == grapheme_splitting::east_asian_width_class_type::neutral);
                return grapheme_splitting::character_width::class_type::neutral;
            }
        }

        static grapheme_splitting::character_width::emoji_type to_emoji_type(const grapheme_splitting::emoji_type emoji)
        {
            switch (emoji)
            {
            case grapheme_splitting::emoji_type::emoji:
                return grapheme_splitting::character_width::emoji_type::emoji;
            default:
                assert(emoji == grapheme_splitting::emoji_type::normal);
                return grapheme_splitting::character_width::emoji_type::normal;
            }
        }

        static grapheme_splitting::grapheme_segment::break_property_type
        to_break_property(const grapheme_splitting::grapheme_break_property_type grapheme)
        {
            switch (grapheme)
            {
            case grapheme_splitting::grapheme_break_property_type::cr:
                return grapheme_splitting::grapheme_segment::break_property_type::cr;
            case grapheme_splitting::grapheme_break_property_type::lf:
                return grapheme_splitting::grapheme_segment::break_property_type::lf;
            case grapheme_splitting::grapheme_break_property_type::control:
                return grapheme_splitting::grapheme_segment::break_property_type::control;
            case grapheme_splitting::grapheme_break_property_type::extend:
                return grapheme_splitting::grapheme_segment::break_property_type::extend;
            case grapheme_splitting::grapheme_break_property_type::zwj:
                return grapheme_splitting::grapheme_segment::break_property_type::zwj;
            case grapheme_splitting::grapheme_break_property_type::regional:
                return grapheme_splitting::grapheme_segment::break_property_type::regional;
            case grapheme_splitting::grapheme_break_property_type::prepend:
                return grapheme_splitting::grapheme_segment::break_property_type::prepend;
            case grapheme_splitting::grapheme_break_property_type::spacing_mark:
                return grapheme_splitting::grapheme_segment::break_property_type::spacing_mark;
            case grapheme_splitting::grapheme_break_property_type::l:
                return grapheme_splitting::grapheme_segment::break_property_type::l;
            case grapheme_splitting::grapheme_break_property_type::v:
                return grapheme_splitting::grapheme_segment::break_property_type::v;
            case grapheme_splitting::grapheme_break_property_type::t:
                return grapheme_splitting::grapheme_segment::break_property_type::t;
            case grapheme_splitting::grapheme_break_property_type::lv:
                return grapheme_splitting::grapheme_segment::break_property_type::lv;
            case grapheme_splitting::grapheme_break_property_type::lvt:
                return grapheme_splitting::grapheme_segment::break_property_type::lvt;
            default:
                assert(grapheme == grapheme_splitting::grapheme_break_property_type::other);
                return grapheme_splitting::grapheme_segment::break_property_type::other;
            }
        }


        // variables

        const grapheme_splitting::character_width& m_character_width;
    };


    grapheme_splitter::grapheme_splitter(const std::locale& locale_ /*= std::locale{ "" }*/) :
    m_p_impl{ std::make_unique<impl>(locale_) }
    {}

    grapheme_splitter::~grapheme_splitter() = default;

    std::vector<grapheme> grapheme_splitter::split(const std::string_view& string_) const
    {
        return m_p_impl->split(string_);
    }


}
