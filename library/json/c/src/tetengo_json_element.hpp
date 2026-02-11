/*! \file
    \brief An element.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_ELEMENT_HPP_)
#define TETENGO_JSON_ELEMENT_HPP_

#include <memory>
#include <utility>

#include <tetengo/json/element.h>
#include <tetengo/json/element.hpp>


struct tetengo_json_element_tag
{
    std::unique_ptr<tetengo::json::element> p_cpp_element_holder;

    const tetengo::json::element* p_cpp_element;

    tetengo_json_element_type_t type;

    mutable tetengo_json_fileLocation_t file_location;

    explicit tetengo_json_element_tag(std::unique_ptr<tetengo::json::element>&& p_cpp_element) :
    p_cpp_element_holder{ std::move(p_cpp_element) },
    p_cpp_element{ std::to_address(p_cpp_element_holder) },
    type{},
    file_location{}
    {}

    explicit tetengo_json_element_tag(const tetengo::json::element* const p_cpp_element) :
    p_cpp_element_holder{},
    p_cpp_element{ p_cpp_element },
    type{},
    file_location{}
    {}
};


#endif
