/*! \file
    \brief An element.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include <stddef.h>

#include <tetengo/json/element.h>
#include <tetengo/json/element.hpp>
#include <tetengo/json/file_location.hpp>

#include "tetengo_json_element.hpp"


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
    const size_t                                          attribute_count,
    const tetengo_json_fileLocation_t* const              p_file_location)
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
        if (!p_file_location)
        {
            throw std::invalid_argument{ "p_file_location is NULL." };
        }

        tetengo::json::element::type_type cpp_type_type{
            static_cast<tetengo::json::element::type_name_type>(p_type->name),
            static_cast<tetengo::json::element::type_category_type>(p_type->category)
        };

        std::unordered_map<std::string, std::string> cpp_attributes{};
        cpp_attributes.reserve(attribute_count);
        std::for_each(p_attributes, p_attributes + attribute_count, [&cpp_attributes](const auto& key_value) {
            cpp_attributes.emplace(key_value.key, key_value.value);
        });

        tetengo::json::file_location cpp_file_location{ p_file_location->line,
                                                        p_file_location->line_index,
                                                        p_file_location->column_index };

        auto p_cpp_element = std::make_unique<tetengo::json::element>(
            std::move(cpp_type_type), value, std::move(cpp_attributes), std::move(cpp_file_location));

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
            throw std::invalid_argument{ "p_element is NULL." };
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

const char* tetengo_json_element_value(const tetengo_json_element_t* const p_element)
{
    try
    {
        if (!p_element)
        {
            throw std::invalid_argument{ "p_element is NULL." };
        }

        return p_element->p_cpp_element->value().c_str();
    }
    catch (...)
    {
        return nullptr;
    }
}

size_t tetengo_json_element_attributeKeys(const tetengo_json_element_t* const p_element, const char** const p_keys)
{
    try
    {
        if (!p_element)
        {
            throw std::invalid_argument{ "p_element is NULL." };
        }

        if (p_keys)
        {
            auto index = static_cast<std::size_t>(0);
            for (const auto& key_value: p_element->p_cpp_element->attributes())
            {
                p_keys[index] = key_value.first.c_str();
                ++index;
            }
        }
        return std::size(p_element->p_cpp_element->attributes());
    }
    catch (...)
    {
        return 0;
    }
}

const char* tetengo_json_element_attributeValueOf(const tetengo_json_element_t* const p_element, const char* const key)
{
    try
    {
        if (!p_element)
        {
            throw std::invalid_argument{ "p_element is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }

        const auto found = p_element->p_cpp_element->attributes().find(key);
        if (found == std::end(p_element->p_cpp_element->attributes()))
        {
            throw std::invalid_argument{ "key is not found." };
        }

        return found->second.c_str();
    }
    catch (...)
    {
        return nullptr;
    }
}

const tetengo_json_fileLocation_t* tetengo_json_element_getFileLocation(const tetengo_json_element_t* const p_element)
{
    try
    {
        if (!p_element)
        {
            throw std::invalid_argument{ "p_element is NULL." };
        }

        const auto& cpp_file_location = p_element->p_cpp_element->get_file_location();
        p_element->file_location.line = cpp_file_location.line().c_str();
        p_element->file_location.line_index = cpp_file_location.line_index();
        p_element->file_location.column_index = cpp_file_location.column_index();
        return &p_element->file_location;
    }
    catch (...)
    {
        return nullptr;
    }
}
