/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <clocale>
#include <cstddef>
#include <iterator>
#include <locale>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <tetengo/text/graphemeSplitter.h>
#include <tetengo/text/grapheme_splitter.hpp>


struct tetengo_text_graphemeSplitter_tag
{
    std::unique_ptr<tetengo::text::grapheme_splitter> p_cpp_grapheme_splitter;

    explicit tetengo_text_graphemeSplitter_tag(
        std::unique_ptr<tetengo::text::grapheme_splitter>&& p_cpp_grapheme_splitter) :
    p_cpp_grapheme_splitter{ std::move(p_cpp_grapheme_splitter) }
    {}
};


tetengo_text_graphemeSplitter_t* tetengo_text_graphemeSplitter_create()
{
    try
    {
        const auto* const c_locale_name = std::setlocale(LC_CTYPE, nullptr);
        assert(c_locale_name);

        auto p_cpp_graph_splitter = std::make_unique<tetengo::text::grapheme_splitter>(std::locale{ c_locale_name });

        auto p_instance = std::make_unique<tetengo_text_graphemeSplitter_t>(std::move(p_cpp_graph_splitter));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_text_graphemeSplitter_destroy(const tetengo_text_graphemeSplitter_t* const p_grapheme_splitter)
{
    try
    {
        const std::unique_ptr<const tetengo_text_graphemeSplitter_t> p_instance{ p_grapheme_splitter };
    }
    catch (...)
    {}
}

size_t tetengo_text_graphemeSplitter_split(
    const tetengo_text_graphemeSplitter_t* const p_grapheme_splitter,
    const char* const                            string,
    tetengo_text_grapheme_t* const               p_graphemes)
{
    try
    {
        if (!p_grapheme_splitter)
        {
            throw std::invalid_argument{ "p_grapheme_spliter is NULL." };
        }
        if (!string)
        {
            throw std::invalid_argument{ "string is NULL." };
        }

        const auto cpp_graphemes = p_grapheme_splitter->p_cpp_grapheme_splitter->split(string);
        if (p_graphemes)
        {
            for (auto i = static_cast<std::size_t>(0); i < std::size(cpp_graphemes); ++i)
            {
                p_graphemes[i].offset = cpp_graphemes[i].offset();
                p_graphemes[i].width = cpp_graphemes[i].width();
            }
        }

        return std::size(cpp_graphemes);
    }
    catch (...)
    {
        return 0;
    }
}
