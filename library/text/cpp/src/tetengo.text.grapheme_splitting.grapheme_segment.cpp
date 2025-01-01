/*! \file
    \brief A grapheme segment.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/text/grapheme_splitting/grapheme_segment.hpp>


namespace tetengo::text::grapheme_splitting
{
    class grapheme_segment::impl : private boost::noncopyable
    {
    public:
        // static functions

        static const grapheme_segment& instance()
        {
            static const grapheme_segment singleton{};
            return singleton;
        }


        // functions

        std::vector<std::size_t> segment_offsets(const std::vector<break_property_type>& break_properties) const
        {
            if (std::empty(break_properties))
            {
                return std::vector<std::size_t>{};
            }

            std::vector<std::size_t> offsets{ 0 };
            auto                     continuous_regional_count = static_cast<std::size_t>(0);
            for (std::size_t i = 1; i < std::size(break_properties); ++i)
            {
                if (connecting_pair_set().find(std::make_pair(break_properties[i - 1], break_properties[i])) !=
                        std::end(connecting_pair_set()) &&
                    (break_properties[i] != break_property_type::regional || continuous_regional_count % 2 == 0))
                {
                    if (break_properties[i - 1] == break_property_type::regional)
                    {
                        ++continuous_regional_count;
                    }
                    else
                    {
                        continuous_regional_count = 0;
                    }
                    continue;
                }

                continuous_regional_count = 0;
                offsets.push_back(i);
            }
            offsets.push_back(std::size(break_properties));

            return offsets;
        }


    private:
        // types

        struct grapheme_pair_hash_type
        {
            std::size_t operator()(const std::pair<break_property_type, break_property_type>& pair) const
            {
                static const std::hash<break_property_type> hash{};
                return hash(pair.first) + hash(pair.second);
            }
        };

        using connecting_pair_set_type =
            std::unordered_set<std::pair<break_property_type, break_property_type>, grapheme_pair_hash_type>;


        // static functions

        static const connecting_pair_set_type& connecting_pair_set()
        {
            static const connecting_pair_set_type singleton = make_connecting_pair_set();
            return singleton;
        }

        static connecting_pair_set_type make_connecting_pair_set()
        {
            using bp = break_property_type;
            return connecting_pair_set_type{
                { bp::cr, bp::lf },
                { bp::extend, bp::extend },
                { bp::extend, bp::zwj },
                { bp::extend, bp::spacing_mark },
                { bp::zwj, bp::extend },
                { bp::zwj, bp::zwj },
                { bp::zwj, bp::spacing_mark },
                { bp::zwj, bp::other },
                { bp::regional, bp::extend },
                { bp::regional, bp::zwj },
                { bp::regional, bp::regional },
                { bp::regional, bp::spacing_mark },
                { bp::prepend, bp::extend },
                { bp::prepend, bp::zwj },
                { bp::prepend, bp::regional },
                { bp::prepend, bp::prepend },
                { bp::prepend, bp::spacing_mark },
                { bp::prepend, bp::l },
                { bp::prepend, bp::v },
                { bp::prepend, bp::t },
                { bp::prepend, bp::lv },
                { bp::prepend, bp::lvt },
                { bp::prepend, bp::other },
                { bp::spacing_mark, bp::extend },
                { bp::spacing_mark, bp::zwj },
                { bp::spacing_mark, bp::spacing_mark },
                { bp::l, bp::extend },
                { bp::l, bp::zwj },
                { bp::l, bp::spacing_mark },
                { bp::l, bp::l },
                { bp::l, bp::v },
                { bp::l, bp::lv },
                { bp::l, bp::lvt },
                { bp::v, bp::extend },
                { bp::v, bp::zwj },
                { bp::v, bp::spacing_mark },
                { bp::v, bp::v },
                { bp::v, bp::t },
                { bp::t, bp::extend },
                { bp::t, bp::zwj },
                { bp::t, bp::spacing_mark },
                { bp::t, bp::t },
                { bp::lv, bp::extend },
                { bp::lv, bp::zwj },
                { bp::lv, bp::spacing_mark },
                { bp::lv, bp::v },
                { bp::lv, bp::t },
                { bp::lvt, bp::extend },
                { bp::lvt, bp::zwj },
                { bp::lvt, bp::spacing_mark },
                { bp::lvt, bp::t },
                { bp::other, bp::extend },
                { bp::other, bp::zwj },
                { bp::other, bp::spacing_mark },
            };
        }
    };


    const grapheme_segment& grapheme_segment::instance()
    {
        return impl::instance();
    }

    grapheme_segment::~grapheme_segment() = default;

    std::vector<std::size_t>
    grapheme_segment::segment_offsets(const std::vector<break_property_type>& break_properties) const
    {
        return m_p_impl->segment_offsets(break_properties);
    }

    grapheme_segment::grapheme_segment() : m_p_impl{ std::make_unique<impl>() } {}


}
