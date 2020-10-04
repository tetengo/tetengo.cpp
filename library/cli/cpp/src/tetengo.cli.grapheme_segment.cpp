/*! \file
    \brief A grapheme segment.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/cli/grapheme_segment.hpp>


namespace tetengo::cli
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

        std::vector<std::size_t> split(const std::vector<grapheme_type>& graphemes) const
        {
            if (graphemes.empty())
            {
                return std::vector<std::size_t>{};
            }

            std::vector<std::size_t> offsets{ 0 };
            auto                     continuous_regional_count = static_cast<std::size_t>(0);
            for (std::size_t i = 1; i < graphemes.size(); ++i)
            {
                if (connecting_pair_set().find(std::make_pair(graphemes[i - 1], graphemes[i])) !=
                        std::end(connecting_pair_set()) &&
                    (graphemes[i] != grapheme_type::regional || continuous_regional_count % 2 == 0))
                {
                    if (graphemes[i - 1] == grapheme_type::regional)
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
            offsets.push_back(graphemes.size());

            return offsets;
        }


    private:
        // types

        struct grapheme_pair_hash_type
        {
            std::size_t operator()(const std::pair<grapheme_type, grapheme_type>& pair) const
            {
                static const std::hash<grapheme_type> hash{};
                return hash(pair.first) + hash(pair.second);
            }
        };

        using connecting_pair_set_type =
            std::unordered_set<std::pair<grapheme_type, grapheme_type>, grapheme_pair_hash_type>;


        // static functions

        static const connecting_pair_set_type& connecting_pair_set()
        {
            static const connecting_pair_set_type singleton = make_connecting_pair_set();
            return singleton;
        }

        static connecting_pair_set_type make_connecting_pair_set()
        {
            using g = grapheme_type;
            return connecting_pair_set_type{
                { g::cr, g::lf },
                { g::extend, g::extend },
                { g::extend, g::zwj },
                { g::extend, g::spacing_mark },
                { g::zwj, g::extend },
                { g::zwj, g::zwj },
                { g::zwj, g::spacing_mark },
                { g::zwj, g::other },
                { g::regional, g::extend },
                { g::regional, g::zwj },
                { g::regional, g::regional },
                { g::regional, g::spacing_mark },
                { g::prepend, g::extend },
                { g::prepend, g::zwj },
                { g::prepend, g::regional },
                { g::prepend, g::prepend },
                { g::prepend, g::spacing_mark },
                { g::prepend, g::l },
                { g::prepend, g::v },
                { g::prepend, g::t },
                { g::prepend, g::lv },
                { g::prepend, g::lvt },
                { g::prepend, g::other },
                { g::spacing_mark, g::extend },
                { g::spacing_mark, g::zwj },
                { g::spacing_mark, g::spacing_mark },
                { g::l, g::extend },
                { g::l, g::zwj },
                { g::l, g::spacing_mark },
                { g::l, g::l },
                { g::l, g::v },
                { g::l, g::lv },
                { g::l, g::lvt },
                { g::v, g::extend },
                { g::v, g::zwj },
                { g::v, g::spacing_mark },
                { g::v, g::v },
                { g::v, g::t },
                { g::t, g::extend },
                { g::t, g::zwj },
                { g::t, g::spacing_mark },
                { g::t, g::t },
                { g::lv, g::extend },
                { g::lv, g::zwj },
                { g::lv, g::spacing_mark },
                { g::lv, g::v },
                { g::lv, g::t },
                { g::lvt, g::extend },
                { g::lvt, g::zwj },
                { g::lvt, g::spacing_mark },
                { g::lvt, g::t },
                { g::other, g::extend },
                { g::other, g::zwj },
                { g::other, g::spacing_mark },
            };
        }
    };


    const grapheme_segment& grapheme_segment::instance()
    {
        return impl::instance();
    }

    grapheme_segment::~grapheme_segment() = default;

    std::vector<std::size_t> grapheme_segment::split(const std::vector<grapheme_type>& graphemes) const
    {
        return m_p_impl->split(graphemes);
    }

    grapheme_segment::grapheme_segment() : m_p_impl{ std::make_unique<impl>() } {}


}
