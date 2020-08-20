/*! \file
    \brief An element.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

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
    {}

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
