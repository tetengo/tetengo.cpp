/*! \file
    \brief An element.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <tetengo/json/element.h>
#include <tetengo/json/element.hpp>


int tetengo_json_element_typeName_string()
{
    return static_cast<int>(tetengo::json::element::type_name_type::string);
}

int tetengo_json_element_typeName_number()
{
    return static_cast<int>(tetengo::json::element::type_name_type::number);
}

int tetengo_json_element_typeName_boolean()
{
    return static_cast<int>(tetengo::json::element::type_name_type::boolean);
}

int tetengo_json_element_typeName_null()
{
    return static_cast<int>(tetengo::json::element::type_name_type::null);
}

int tetengo_json_element_typeName_object()
{
    return static_cast<int>(tetengo::json::element::type_name_type::object);
}

int tetengo_json_element_typeName_member()
{
    return static_cast<int>(tetengo::json::element::type_name_type::member);
}

int tetengo_json_element_typeName_array()
{
    return static_cast<int>(tetengo::json::element::type_name_type::array);
}

int tetengo_json_element_typeCategory_primitive()
{
    return static_cast<int>(tetengo::json::element::type_category_type::primitive);
}

int tetengo_json_element_typeCategory_structureOpen()
{
    return static_cast<int>(tetengo::json::element::type_category_type::structure_open);
}

int tetengo_json_element_typeCategory_structureClose()
{
    return static_cast<int>(tetengo::json::element::type_category_type::structure_close);
}
