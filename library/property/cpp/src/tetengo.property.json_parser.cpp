/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/element.hpp>
#include <tetengo/json/json_parser.hpp>
#include <tetengo/text/encoder.hpp>
#include <tetengo/text/encoding/utf16.hpp> // IWYU pragma: keep

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
            if (name_found == std::end(attributes))
            {
                return false;
            }
            const auto key = key_prefix / unescape(name_found->second);
            parser.next();

            if (next_is_boolean(parser))
            {
                if (!parse_boolean(parser, value_map, key))
                {
                    return false;
                }
            }
            else if (next_is_number(parser))
            {
                if (!parse_number(parser, value_map, key))
                {
                    return false;
                }
            }
            else if (next_is_string(parser))
            {
                if (!parse_string(parser, value_map, key))
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
            else if (next_is_primitive(parser))
            {
                parser.next();
            }
            else if (next_is_structure_open(parser))
            {
                if (!skip_structure(parser))
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
        parse_boolean(tetengo::json::json_parser& parser, value_map_type& value_map, const std::filesystem::path& key)
        {
            assert(next_is_boolean(parser));
            const auto& boolean_element = parser.peek();
            const auto  o_value = to_bool(boolean_element.value());
            if (!o_value)
            {
                return false;
            }
            value_map.insert(std::make_pair(key.string(), *o_value));
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

        static bool
        parse_string(tetengo::json::json_parser& parser, value_map_type& value_map, const std::filesystem::path& key)
        {
            assert(next_is_string(parser));
            const auto& string_element = parser.peek();
            value_map.insert(std::make_pair(key.string(), unescape(string_element.value())));
            parser.next();

            return true;
        }

        static bool skip_structure(tetengo::json::json_parser& parser)
        {
            assert(next_is_structure_open(parser));
            parser.next();

            std::size_t level = 1;
            while (level > 0 && parser.has_next())
            {
                if (next_is_structure_open(parser))
                {
                    ++level;
                }
                else if (next_is_structure_close(parser))
                {
                    --level;
                }
                parser.next();
            }

            return level == 0;
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

        static bool next_is_boolean(const tetengo::json::json_parser& parser)
        {
            return next_is(parser, element_type::type_name_type::boolean, element_type::type_category_type::primitive);
        }

        static bool next_is_number(const tetengo::json::json_parser& parser)
        {
            return next_is(parser, element_type::type_name_type::number, element_type::type_category_type::primitive);
        }

        static bool next_is_string(const tetengo::json::json_parser& parser)
        {
            return next_is(parser, element_type::type_name_type::string, element_type::type_category_type::primitive);
        }

        static bool next_is_primitive(const tetengo::json::json_parser& parser)
        {
            return next_is(parser, element_type::type_category_type::primitive);
        }

        static bool next_is_structure_open(const tetengo::json::json_parser& parser)
        {
            return next_is(parser, element_type::type_category_type::structure_open);
        }

        static bool next_is_structure_close(const tetengo::json::json_parser& parser)
        {
            return next_is(parser, element_type::type_category_type::structure_close);
        }

        static bool next_is(
            const tetengo::json::json_parser&      parser,
            const element_type::type_name_type     name,
            const element_type::type_category_type category)
        {
            return next_is(parser, category) && parser.peek().type().name == name;
        }

        static bool next_is(const tetengo::json::json_parser& parser, const element_type::type_category_type category)
        {
            if (!parser.has_next())
            {
                return false;
            }
            const auto& next = parser.peek();
            return next.type().category == category;
        }

        static std::optional<bool> to_bool(const std::string& string)
        {
            if (string == "true")
            {
                return std::make_optional(true);
            }
            else if (string == "false")
            {
                return std::make_optional(false);
            }
            else
            {
                return std::nullopt;
            }
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

        static std::string unescape(const std::string_view& escaped)
        {
            std::string unescaped{};
            for (auto i = static_cast<std::size_t>(0); i < escaped.length();)
            {
                if (escaped[i] == '\\')
                {
                    ++i;
                    if (i < escaped.length())
                    {
                        if (escaped[i] == 'u' && i + 4 < escaped.length())
                        {
                            const auto utf16_unescaped = unescape_utf16(escaped, i);
                            unescaped += utf16_unescaped.first;
                            i += utf16_unescaped.second;
                        }
                        else
                        {
                            unescaped += unescape_control_code(escaped[i]);
                            ++i;
                        }
                    }
                }
                else
                {
                    unescaped += escaped[i];
                    ++i;
                }
            }
            return unescaped;
        }

        static std::pair<std::string, std::size_t>
        unescape_utf16(const std::string_view& escaped, const std::size_t offset)
        {
            auto index = offset;

            ++index;
            const auto upper_code_string = escaped.substr(index, 4);
            const auto o_upper_code = to_utf16_code(upper_code_string);
            index += 4;
            if (!o_upper_code)
            {
                return std::make_pair(std::string{ upper_code_string }, index - offset);
            }
            else if (
                is_high_surrogate(*o_upper_code) && index + 5 < escaped.length() && escaped[index] == '\\' &&
                escaped[index + 1] == 'u')
            {
                index += 2;
                const auto lower_code_string = escaped.substr(index, 4);
                const auto o_lower_code = to_utf16_code(lower_code_string);
                index += 4;
                if (!o_lower_code)
                {
                    return std::make_pair(
                        unescape_utf16_codes(*o_upper_code) + std::string{ lower_code_string }, index - offset);
                }
                else
                {
                    return std::make_pair(unescape_utf16_codes(*o_upper_code, *o_lower_code), index - offset);
                }
            }
            else
            {
                return std::make_pair(unescape_utf16_codes(*o_upper_code), index - offset);
            }
        }

        static std::optional<char16_t> to_utf16_code(const std::string_view& string)
        {
            try
            {
                return std::make_optional(static_cast<char16_t>(std::stoul(std::string{ string }, nullptr, 16)));
            }
            catch (const std::invalid_argument&)
            {
                return std::nullopt;
            }
        }

        static bool is_high_surrogate(const char16_t code)
        {
            return 0xD800 <= code && code <= 0xDBFF;
        }

        static std::string unescape_utf16_codes(const char16_t upper_code, const char16_t lower_code = 0)
        {
            static const auto& encoder = tetengo::text::encoder<tetengo::text::encoding::utf16>::instance();

            std::u16string utf16{ upper_code };
            if (lower_code > 0)
            {
                utf16 += lower_code;
            }
            return encoder.decode(utf16);
        }

        static char unescape_control_code(const char escaped)
        {
            switch (escaped)
            {
            case '"':
                return '"';
            case '\\':
                return '\\';
            case '/':
                return '/';
            case 'b':
                return '\b';
            case 'f':
                return '\f';
            case 'n':
                return '\n';
            case 'r':
                return '\r';
            case 't':
                return '\t';
            default:
                return escaped;
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
