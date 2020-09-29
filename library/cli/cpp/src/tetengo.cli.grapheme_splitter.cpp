/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <memory>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/cli/grapheme_splitter.hpp>


namespace tetengo::cli
{
    class grapheme_splitter::impl : private boost::noncopyable
    {
    public:
        // static functions

        static const grapheme_splitter& instance()
        {
            static const grapheme_splitter singleton{};
            return singleton;
        }


        // functions

        std::vector<std::size_t> split(const std::vector<char32_t>& /*code_points*/) const
        {
            std::vector<std::size_t> offsets{};
            return offsets;
        }


    private:
        // static functions
    };


    const grapheme_splitter& grapheme_splitter::instance()
    {
        return impl::instance();
    }

    grapheme_splitter::~grapheme_splitter() = default;

    std::vector<std::size_t> grapheme_splitter::split(const std::vector<char32_t>& code_points) const
    {
        return m_p_impl->split(code_points);
    }

    grapheme_splitter::grapheme_splitter() : m_p_impl{ std::make_unique<impl>() } {}


}
