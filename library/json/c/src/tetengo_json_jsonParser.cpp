/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <stdexcept>
#include <utility>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>

#include <tetengo/json/element.h>
#include <tetengo/json/jsonParser.h>
#include <tetengo/json/json_parser.hpp>
#include <tetengo/json/reader.h>

#include "tetengo_json_element.hpp"
#include "tetengo_json_reader.hpp"


struct tetengo_json_jsonParser_tag
{
    std::unique_ptr<tetengo::json::json_parser> p_cpp_parser;

    mutable std::unique_ptr<const tetengo_json_element_t> p_current_element;

    explicit tetengo_json_jsonParser_tag(std::unique_ptr<tetengo::json::json_parser>&& p_cpp_parser) :
    p_cpp_parser{ std::move(p_cpp_parser) },
    p_current_element{}
    {}
};


size_t tetengo_json_jsonParser_defaultBufferCapacity(void)
{
    try
    {
        return tetengo::json::json_parser::default_buffer_capacity();
    }
    catch (...)
    {
        return 0;
    }
}

tetengo_json_jsonParser_t*
tetengo_json_jsonParser_create(tetengo_json_reader_t* const p_reader, const size_t buffer_capacity)
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

        auto p_cpp_parser =
            std::make_unique<tetengo::json::json_parser>(std::move(p_reader->move_cpp_reader()), buffer_capacity);

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

int tetengo_json_jsonParser_hasNext(const tetengo_json_jsonParser_t* const p_parser)
{
    try
    {
        if (!p_parser)
        {
            throw std::invalid_argument{ "p_parser is NULL." };
        }

        return p_parser->p_cpp_parser->has_next() ? 1 : 0;
    }
    catch (...)
    {
        return 0;
    }
}

const tetengo_json_element_t* tetengo_json_jsonParser_peek(const tetengo_json_jsonParser_t* const p_parser)
{
    try
    {
        if (!p_parser)
        {
            throw std::invalid_argument{ "p_parser is NULL." };
        }

        p_parser->p_current_element = std::make_unique<tetengo_json_element_t>(&p_parser->p_cpp_parser->peek());
        return p_parser->p_current_element.get();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_json_jsonParser_next(tetengo_json_jsonParser_t* const p_parser)
{
    try
    {
        if (!p_parser)
        {
            throw std::invalid_argument{ "p_parser is NULL." };
        }

        p_parser->p_cpp_parser->next();
    }
    catch (...)
    {}
}
