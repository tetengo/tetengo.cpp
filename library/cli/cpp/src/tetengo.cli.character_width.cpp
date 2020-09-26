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

#include "tetengo.cli.east_asian_width.hpp"


namespace tetengo::cli
{
    class character_width::impl : private boost::noncopyable
    {
    public:
        // static functions

        static std::pair<class_type, bool> class_of(const char32_t code_point)
        {
            const auto* p_lower_bound = std::lower_bound(
                east_asian_width_list,
                east_asian_width_list + east_asian_width_list_size,
                east_asian_width_type{ code_point, east_asian_width_class_type::ambiguous, false },
                [](const auto& element1, const auto& element2) { return element1.code_point < element2.code_point; });

            if (p_lower_bound != east_asian_width_list + east_asian_width_list_size &&
                p_lower_bound->code_point == code_point)
            {
                return std::make_pair(to_class_type(p_lower_bound->class_), p_lower_bound->combining);
            }

            assert(p_lower_bound != east_asian_width_list);
            const auto* const p_previous = std::prev(p_lower_bound);
            return std::make_pair(to_class_type(p_previous->class_), p_previous->combining);
        }


        // functions

        std::size_t
        width_of(const char32_t previous_code_point, const char32_t code_point, const character_width& if_) const
        {
            return if_.width_of_impl(previous_code_point, code_point);
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
    };


    character_width::~character_width() = default;

    std::pair<character_width::class_type, bool> character_width::class_of(const char32_t code_point)
    {
        return impl::class_of(code_point);
    }

    character_width::character_width() : m_p_impl{ std::make_unique<impl>() } {}

    std::size_t character_width::width_of(const char32_t previous_code_point, const char32_t code_point) const
    {
        return m_p_impl->width_of(previous_code_point, code_point, *this);
    }


}
