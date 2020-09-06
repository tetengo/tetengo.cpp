/*! \file
    \brief A reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_READER_HPP_)
#define TETENGO_JSON_READER_HPP_

#include <memory>
#include <utility>

#include <tetengo/json/reader.hpp>


struct tetengo_json_reader_tag
{
    std::unique_ptr<tetengo::json::reader> p_cpp_reader;

    tetengo_json_reader_tag(std::unique_ptr<tetengo::json::reader>&& p_cpp_reader) :
    p_cpp_reader{ std::move(p_cpp_reader) }
    {}
};


#endif
