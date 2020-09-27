/*! \file
    \brief A character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/cli/character_width.hpp>

#include "tetengo.cli.character_property_map.hpp"


namespace tetengo::cli
{
    class character_width::impl : private boost::noncopyable
    {
    public:
        // static functions

        static std::pair<class_type, grapheme_type> property_of(const char32_t code_point)
        {
            const auto* p_lower_bound = std::lower_bound(
                character_property_map,
                character_property_map + character_property_map_size,
                character_property_map_element_type{
                    code_point, east_asian_width_class_type::ambiguous, grapheme_break_property::other },
                [](const auto& element1, const auto& element2) { return element1.code_point < element2.code_point; });

            if (p_lower_bound != character_property_map + character_property_map_size &&
                p_lower_bound->code_point == code_point)
            {
                return std::make_pair(to_class_type(p_lower_bound->class_), to_grapheme(p_lower_bound->grapheme));
            }

            assert(p_lower_bound != character_property_map);
            const auto* const p_previous = std::prev(p_lower_bound);
            return std::make_pair(to_class_type(p_previous->class_), to_grapheme(p_previous->grapheme));
        }


        // functions

        std::size_t width_of(const char32_t code_point, const character_width& if_) const
        {
            return if_.width_of_impl(code_point);
        }


    private:
        // static functions

        static class_type to_class_type(const east_asian_width_class_type class_type)
        {
            switch (class_type)
            {
            case east_asian_width_class_type::ambiguous:
                return class_type::ambiguous;
            case east_asian_width_class_type::fullwidth:
                return class_type::fullwidth;
            case east_asian_width_class_type::halfwidth:
                return class_type::halfwidth;
            case east_asian_width_class_type::narrow:
                return class_type::narrow;
            case east_asian_width_class_type::wide:
                return class_type::wide;
            default:
                assert(class_type == east_asian_width_class_type::neutral);
                return class_type::neutral;
            }
        }

        static grapheme_type to_grapheme(const grapheme_break_property grapheme)
        {
            switch (grapheme)
            {
            case grapheme_break_property::cr:
                return grapheme_type::cr;
            case grapheme_break_property::lf:
                return grapheme_type::lf;
            case grapheme_break_property::control:
                return grapheme_type::control;
            case grapheme_break_property::extend:
                return grapheme_type::extend;
            case grapheme_break_property::zwj:
                return grapheme_type::zwj;
            case grapheme_break_property::regional:
                return grapheme_type::regional;
            case grapheme_break_property::prepend:
                return grapheme_type::prepend;
            case grapheme_break_property::spacing_mark:
                return grapheme_type::spacing_mark;
            case grapheme_break_property::l:
                return grapheme_type::l;
            case grapheme_break_property::v:
                return grapheme_type::v;
            case grapheme_break_property::t:
                return grapheme_type::t;
            case grapheme_break_property::lv:
                return grapheme_type::lv;
            case grapheme_break_property::lvt:
                return grapheme_type::lvt;
            default:
                assert(grapheme == grapheme_break_property::other);
                return grapheme_type::other;
            }
        }
    };


    character_width::~character_width() = default;

    std::pair<character_width::class_type, character_width::grapheme_type>
    character_width::property_of(const char32_t code_point)
    {
        return impl::property_of(code_point);
    }

    character_width::character_width() : m_p_impl{ std::make_unique<impl>() } {}

    std::size_t character_width::width_of(const char32_t code_point) const
    {
        return m_p_impl->width_of(code_point, *this);
    }


}
