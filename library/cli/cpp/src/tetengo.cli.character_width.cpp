/*! \file
    \brief A character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/cli/character_width.hpp>

#include "tetengo.cli.east_asian_width.hpp"


namespace tetengo::cli
{
    class character_width::impl : private boost::noncopyable
    {
    public:
        // static functions

        static int class_of(const char32_t code_point)
        {
            const auto* p_lower_bound = std::lower_bound(
                east_asian_width_list,
                east_asian_width_list + east_asian_width_list_size,
                east_asian_width_type{ code_point, 0 },
                [](const auto& element1, const auto& element2) { return element1.code_point < element2.code_point; });

            if (p_lower_bound != east_asian_width_list + east_asian_width_list_size &&
                p_lower_bound->code_point == code_point)
            {
                return p_lower_bound->class_;
            }

            assert(p_lower_bound != east_asian_width_list);
            return std::prev(p_lower_bound)->class_;
        }

        static bool is_combining(const int class_)
        {
            return class_ < 0;
        }


        // functions

        std::size_t
        width_of(const char32_t previous_code_point, const char32_t code_point, const character_width& if_) const
        {
            return if_.width_of_impl(previous_code_point, code_point);
        }
    };


    character_width::~character_width() = default;

    int character_width::class_of(const char32_t code_point)
    {
        return impl::class_of(code_point);
    }

    bool character_width::is_combining(const int class_)
    {
        return impl::is_combining(class_);
    }

    character_width::character_width() : m_p_impl{ std::make_unique<impl>() } {}

    std::size_t character_width::width_of(const char32_t previous_code_point, const char32_t code_point) const
    {
        return m_p_impl->width_of(previous_code_point, code_point, *this);
    }


}
