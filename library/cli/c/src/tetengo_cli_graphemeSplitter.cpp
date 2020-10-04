/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <clocale>
#include <locale>
#include <memory>
#include <utility>

#include <tetengo/cli/graphemeSplitter.h>
#include <tetengo/cli/grapheme_splitter.hpp>


struct tetengo_cli_graphemeSplitter_tag
{
    std::unique_ptr<tetengo::cli::grapheme_splitter> p_cpp_grapheme_splitter;

    tetengo_cli_graphemeSplitter_tag(std::unique_ptr<tetengo::cli::grapheme_splitter>&& p_cpp_grapheme_splitter) :
    p_cpp_grapheme_splitter{ std::move(p_cpp_grapheme_splitter) }
    {}
};


tetengo_cli_graphemeSplitter_t* tetengo_cli_graphemeSplitter_create()
{
    try
    {
        const auto* const c_locale_name = std::setlocale(LC_CTYPE, nullptr);
        assert(c_locale_name);

        auto p_cpp_graph_splitter = std::make_unique<tetengo::cli::grapheme_splitter>(std::locale{ c_locale_name });

        auto p_instance = std::make_unique<tetengo_cli_graphemeSplitter_t>(std::move(p_cpp_graph_splitter));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_cli_graphemeSplitter_destroy(const tetengo_cli_graphemeSplitter_t* const p_grapheme_splitter)
{
    try
    {
        const std::unique_ptr<const tetengo_cli_graphemeSplitter_t> p_instance{ p_grapheme_splitter };
    }
    catch (...)
    {}
}
