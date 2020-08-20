/*! \file
    \brief An element.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include <tetengo/json/element.hpp>


namespace tetengo::json
{
    element::element(type_type type, std::string value, std::unordered_map<std::string, std::string> attributes) :
    m_type{ std::move(type) },
        m_value{ std::move(value) },
        m_attributes{ std::move(attributes) }
    {
        if (m_type.name == type_name_type::string || m_type.name == type_name_type::number ||
            m_type.name == type_name_type::boolean || m_type.name == type_name_type::null)
        {
            if (m_type.category != type_category_type::primitive)
            {
                throw std::invalid_argument{ "Mismatch between type name and type category." };
            }
            if (!m_attributes.empty())
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
            if (!m_value.empty())
            {
                throw std::invalid_argument{ "Mismatch between type and value." };
            }
            if (m_type.name == type_name_type::member && m_type.category == type_category_type::structure_open)
            {
                if (m_attributes.size() != 1U || m_attributes.find("name") == m_attributes.end())
                {
                    throw std::invalid_argument{ "Mismatch between type and attributes." };
                }
            }
            else
            {
                if (!m_attributes.empty())
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


}
