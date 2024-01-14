/*! \file
    \brief An element.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#include <iterator>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include <tetengo/json/element.hpp>
#include <tetengo/json/file_location.hpp>


namespace tetengo::json
{
    element::element(
        type_type                                    type,
        std::string                                  value,
        std::unordered_map<std::string, std::string> attributes,
        file_location                                file_location_) :
    m_type{ std::move(type) },
    m_value{ std::move(value) },
    m_attributes{ std::move(attributes) },
    m_file_location{ std::move(file_location_) }
    {
        if (m_type.name == type_name_type::string || m_type.name == type_name_type::number ||
            m_type.name == type_name_type::boolean || m_type.name == type_name_type::null)
        {
            if (m_type.category != type_category_type::primitive)
            {
                throw std::invalid_argument{ "Mismatch between type name and type category." };
            }
            if (!std::empty(m_attributes))
            {
                throw std::invalid_argument{ "Mismatch between type and attributes." };
            }
        }
        else
        {
            if (m_type.category == type_category_type::primitive)
            {
                throw std::invalid_argument{ "Mismatch between type name and type category." };
            }
            if (!std::empty(m_value))
            {
                throw std::invalid_argument{ "Mismatch between type and value." };
            }
            if (m_type.name == type_name_type::member && m_type.category == type_category_type::structure_open)
            {
                if (std::size(m_attributes) != 1U || m_attributes.find("name") == std::end(m_attributes))
                {
                    throw std::invalid_argument{ "Mismatch between type and attributes." };
                }
            }
            else
            {
                if (!std::empty(m_attributes))
                {
                    throw std::invalid_argument{ "Mismatch between type and attributes." };
                }
            }
        }
    }

    const element::type_type& element::type() const
    {
        return m_type;
    }

    const std::string& element::value() const
    {
        return m_value;
    }

    const std::unordered_map<std::string, std::string>& element::attributes() const
    {
        return m_attributes;
    }

    const file_location& element::get_file_location() const
    {
        return m_file_location;
    }


}
