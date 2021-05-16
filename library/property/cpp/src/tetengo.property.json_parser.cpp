/*! \file
    \brief A json parser.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/element.hpp>
#include <tetengo/json/json_parser.hpp>

#include "tetengo.property.json_parser.hpp"


namespace tetengo::property
{
    class json_parser::impl : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = json_parser::value_map_type;


        // static functions

        static const json_parser& instance()
        {
            static const json_parser singleton{};
            return singleton;
        }


        // functions

        value_map_type parse(tetengo::json::json_parser& parser) const
        {
            value_map_type value_map{};
            parse_object(parser, value_map, std::filesystem::path{});
            return value_map;
        }


    private:
        // types

        using element_type = tetengo::json::element;


        // static functions

        static bool parse_object(
            tetengo::json::json_parser&  parser,
            value_map_type&              value_map,
            const std::filesystem::path& key_prefix)
        {
            if (!next_is_object_open(parser))
            {
                return false;
            }
            parser.next();

            while (next_is_member_open(parser))
            {
                if (!parse_member(parser, value_map, key_prefix))
                {
                    return false;
                }
            }

            if (!next_is_object_close(parser))
            {
                return false;
            }
            parser.next();

            return true;
        }

        static bool parse_member(
            tetengo::json::json_parser&  parser,
            value_map_type&              value_map,
            const std::filesystem::path& key_prefix)
        {
            assert(next_is_member_open(parser));
            const auto& member_element = parser.peek();
            const auto& attributes = member_element.attributes();
            const auto  name_found = attributes.find("name");
            if (name_found == attributes.end())
            {
                return false;
            }
            const auto key = key_prefix / name_found->second;
            parser.next();

            if (next_is_number(parser))
            {
                if (!parse_number(parser, value_map, key))
                {
                    return false;
                }
            }
            else if (next_is_object_open(parser))
            {
                if (!parse_object(parser, value_map, key))
                {
                    return false;
                }
            }

            if (!next_is_member_close(parser))
            {
                return false;
            }
            parser.next();

            return true;
        }

        static bool
        parse_number(tetengo::json::json_parser& parser, value_map_type& value_map, const std::filesystem::path& key)
        {
            assert(next_is_number(parser));
            const auto& number_element = parser.peek();
            const auto  o_value = to_integer(number_element.value());
            if (!o_value)
            {
                return false;
            }
            value_map.insert(std::make_pair(key.string(), *o_value));
            parser.next();

            return true;
        }

        static bool next_is_object_open(const tetengo::json::json_parser& parser)
        {
            return next_is(
                parser, element_type::type_name_type::object, element_type::type_category_type::structure_open);
        }

        static bool next_is_object_close(const tetengo::json::json_parser& parser)
        {
            return next_is(
                parser, element_type::type_name_type::object, element_type::type_category_type::structure_close);
        }

        static bool next_is_member_open(const tetengo::json::json_parser& parser)
        {
            return next_is(
                parser, element_type::type_name_type::member, element_type::type_category_type::structure_open);
        }

        static bool next_is_member_close(const tetengo::json::json_parser& parser)
        {
            return next_is(
                parser, element_type::type_name_type::member, element_type::type_category_type::structure_close);
        }

        static bool next_is_number(const tetengo::json::json_parser& parser)
        {
            return next_is(parser, element_type::type_name_type::number, element_type::type_category_type::primitive);
        }

        static bool next_is(
            const tetengo::json::json_parser&      parser,
            const element_type::type_name_type     name,
            const element_type::type_category_type category)
        {
            if (!parser.has_next())
            {
                return false;
            }
            const auto& next = parser.peek();
            return next.type().name == name && next.type().category == category;
        }

        static std::optional<std::uint32_t> to_integer(const std::string& string)
        {
            try
            {
                return std::make_optional(std::stoul(string));
            }
            catch (const std::invalid_argument&)
            {
                return std::nullopt;
            }
        }
    };


    const json_parser& json_parser::instance()
    {
        return impl::instance();
    }

    json_parser::~json_parser() = default;

    json_parser::value_map_type json_parser::parse(tetengo::json::json_parser& parser) const
    {
        return m_p_impl->parse(parser);
    }

    json_parser::json_parser() : m_p_impl{ std::make_unique<impl>() } {}
}
