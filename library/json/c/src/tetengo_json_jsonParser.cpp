/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <stdexcept>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>

#include <tetengo/json/jsonParser.h>
#include <tetengo/json/json_parser.hpp>
#include <tetengo/json/reader.h>

#include "tetengo_json_reader.hpp" // IWYU pragma: keep


struct tetengo_json_jsonParser_tag
{
    std::unique_ptr<tetengo::json::json_parser> p_cpp_parser;

    tetengo_json_jsonParser_tag(std::unique_ptr<tetengo::json::json_parser>&& p_cpp_parser) :
    p_cpp_parser{ std::move(p_cpp_parser) }
    {}
};


tetengo_json_jsonParser_t* tetengo_json_jsonParser_create(tetengo_json_reader_t* const p_reader)
{
    try
    {
        BOOST_SCOPE_EXIT(p_reader)
        {
            tetengo_json_reader_destroy(p_reader);
        }
        BOOST_SCOPE_EXIT_END;

        if (!p_reader)
        {
            throw std::invalid_argument{ "p_reader is NULL." };
        }

        auto p_cpp_parser = std::make_unique<tetengo::json::json_parser>(std::move(p_reader->p_cpp_reader));

        auto p_instance = std::make_unique<tetengo_json_jsonParser_t>(std::move(p_cpp_parser));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_json_jsonParser_destroy(const tetengo_json_jsonParser_t* const p_parser)
{
    try
    {
        const std::unique_ptr<const tetengo_json_jsonParser_t> p_instance{ p_parser };
    }
    catch (...)
    {}
}
