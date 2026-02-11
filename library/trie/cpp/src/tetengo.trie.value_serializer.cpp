/*! \file
    \brief A value serializer.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

#include <tetengo/trie/value_serializer.hpp>


namespace tetengo::trie
{
    value_serializer::value_serializer(
        std::function<std::vector<char>(const std::any&)> serialize,
        const std::size_t                                 fixed_value_size) :
    m_serialize{ std::move(serialize) },
    m_fixed_value_size{ fixed_value_size }
    {}

    std::vector<char> value_serializer::operator()(const std::any& value) const
    {
        return m_serialize(value);
    }

    std::size_t value_serializer::fixed_value_size() const
    {
        return m_fixed_value_size;
    }


    value_deserializer::value_deserializer(std::function<std::any(const std::vector<char>&)> deserialize) :
    m_deserialize{ std::move(deserialize) }
    {}

    std::any value_deserializer::operator()(const std::vector<char>& serialized) const
    {
        return m_deserialize(serialized);
    }


}
