/*! \file
    \brief A default character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/cli/default_character_width.hpp>

#include "tetengo.cli.east_asian_width.hpp"


namespace tetengo::cli
{
    class default_character_width::impl : private boost::noncopyable
    {
    public:
        // static functions

        static const default_character_width& instance()
        {
            static const default_character_width singleton{};
            return singleton;
        }


        // functions

        std::size_t width_of_impl(const char32_t previous_code_point, const char32_t code_point) const
        {
            return previous_code_point != 0 && is_combining(code_point) ? 0 : 1;
        }


    private:
        // static functions

        static bool is_combining(const char32_t code_point)
        {
            const auto* p_lower_bound = std::lower_bound(
                east_asian_width_list,
                east_asian_width_list + east_asian_width_list_size,
                east_asian_width_type{ code_point, 0 },
                [](const auto& element1, const auto& element2) { return element1.code_point < element2.code_point; });

            if (p_lower_bound != east_asian_width_list + east_asian_width_list_size &&
                p_lower_bound->code_point == code_point)
            {
                return p_lower_bound->class_ < 0;
            }

            assert(p_lower_bound != east_asian_width_list);
            return std::prev(p_lower_bound)->class_ < 0;
        }
    };


    const default_character_width& default_character_width::instance()
    {
        return impl::instance();
    }

    default_character_width::~default_character_width() = default;

    default_character_width::default_character_width() : m_p_impl{ std::make_unique<impl>() } {}

    std::size_t
    default_character_width::width_of_impl(const char32_t previous_code_point, const char32_t code_point) const
    {
        return m_p_impl->width_of_impl(previous_code_point, code_point);
    }


}
