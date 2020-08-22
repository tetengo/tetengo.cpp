/*! \file
    \brief An element.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include <stddef.h>

#include <tetengo/json/element.h>
#include <tetengo/json/element.hpp>


struct tetengo_json_element_tag
{
    std::unique_ptr<tetengo::json::element> p_cpp_element;

    tetengo_json_element_type_t type;

    tetengo_json_element_tag(std::unique_ptr<tetengo::json::element>&& p_cpp_element) :
    p_cpp_element{ std::move(p_cpp_element) },
        type{}
    {}
};


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

tetengo_json_element_t* tetengo_json_element_create(
    const tetengo_json_element_type_t* const              p_type,
    const char* const                                     value,
    const tetengo_json_element_attributeKeyValue_t* const p_attributes,
    const size_t                                          attribute_count)
{
    try
    {
        if (!p_type)
        {
            throw std::invalid_argument{ "p_type is NULL." };
        }
        if (!value)
        {
            throw std::invalid_argument{ "value is NULL." };
        }
        if (!p_attributes && attribute_count > 0)
        {
            throw std::invalid_argument{ "p_attributes is NULL." };
        }

        const tetengo::json::element::type_type cpp_type_type{
            static_cast<tetengo::json::element::type_name_type>(p_type->name),
            static_cast<tetengo::json::element::type_category_type>(p_type->category)
        };

        std::unordered_map<std::string, std::string> cpp_attributes{};
        cpp_attributes.reserve(attribute_count);
        std::for_each(p_attributes, p_attributes + attribute_count, [&cpp_attributes](const auto& key_value) {
            cpp_attributes.emplace(key_value.key, key_value.value);
        });

        auto p_cpp_element = std::make_unique<tetengo::json::element>(cpp_type_type, value, cpp_attributes);

        auto p_instance = std::make_unique<tetengo_json_element_t>(std::move(p_cpp_element));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_json_element_t* tetengo_json_element_copy(const tetengo_json_element_t* const p_another)
{
    try
    {
        if (!p_another)
        {
            throw std::invalid_argument{ "p_another is NULL." };
        }

        auto p_cpp_element = std::make_unique<tetengo::json::element>(*p_another->p_cpp_element);

        auto p_instance = std::make_unique<tetengo_json_element_t>(std::move(p_cpp_element));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_json_element_destroy(const tetengo_json_element_t* const p_element)
{
    try
    {
        const std::unique_ptr<const tetengo_json_element_t> p_instance{ p_element };
    }
    catch (...)
    {}
}

const tetengo_json_element_type_t* tetengo_json_element_type(const tetengo_json_element_t* const p_element)
{
    try
    {
        if (!p_element)
        {
            throw std::invalid_argument{ "p_another is NULL." };
        }

        auto* const p_mutable_element = const_cast<tetengo_json_element_t*>(p_element);
        p_mutable_element->type.name = static_cast<int>(p_element->p_cpp_element->type().name);
        p_mutable_element->type.category = static_cast<int>(p_element->p_cpp_element->type().category);
        return &p_element->type;
    }
    catch (...)
    {
        return nullptr;
    }
}

size_t tetengo_json_element_value(const tetengo_json_element_t* const p_element, char* const p_value)
{
    try
    {
        if (!p_element)
        {
            throw std::invalid_argument{ "p_another is NULL." };
        }

        if (p_value)
        {
            std::copy(
                std::begin(p_element->p_cpp_element->value()), std::end(p_element->p_cpp_element->value()), p_value);
        }
        return p_element->p_cpp_element->value().length();
    }
    catch (...)
    {
        return 0;
    }
}
