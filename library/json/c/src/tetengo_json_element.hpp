/*! \file
    \brief An element.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
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

    explicit tetengo_json_element_tag(std::unique_ptr<tetengo::json::element>&& p_cpp_element) :
    p_cpp_element_holder{ std::move(p_cpp_element) },
        p_cpp_element{ p_cpp_element_holder.get() },
        type{}
    {}

    explicit tetengo_json_element_tag(const tetengo::json::element* const p_cpp_element) :
    p_cpp_element_holder{},
        p_cpp_element{ p_cpp_element },
        type{}
    {}
};


#endif
