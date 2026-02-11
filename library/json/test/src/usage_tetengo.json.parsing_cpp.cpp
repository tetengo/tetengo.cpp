/*! \file
    \brief The usage of JSON parsing.

    $12026 kaoru  https://www.tetengo.org/
*/

#include "usage_tetengo.json.parsing_cpp.hpp"

// [parsing]
#include <cassert>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <tetengo/json/element.hpp>
#include <tetengo/json/json_parser.hpp>
#include <tetengo/json/reader.hpp>
#include <tetengo/json/stream_reader.hpp>


namespace usage_tetengo::json
{
    std::string to_string(const tetengo::json::element& element_);

    void parsing()
    {
        static const std::string json_text{
            // clang-format off
            "{\n"
            "  \"hoge\": 42,\n"
            "  \"fuga\": [ \"foo\", \"bar\" ]\n"
            "}\n"
            // clang-format on
        };
        auto p_json_stream = std::make_unique<std::istringstream>(json_text);

        // Creates a reader from std::stream.
        auto p_reader = std::make_unique<tetengo::json::stream_reader>(std::move(p_json_stream));

        // Creates a JSON parser.
        tetengo::json::json_parser parser{ std::move(p_reader) };

        // Iteration.
        std::vector<std::string> elements{};
        while (parser.has_next())
        {
            // Obtains the current element.
            const auto& element = parser.peek();

            elements.push_back(to_string(element));

            // Moves to the next element.
            parser.next();
        }

        static const std::vector<std::string> expected{
            // clang-format off
            "object:open:",
            "member:open:name=hoge:",
            "number:42",
            "member:close:",
            "member:open:name=fuga:",
            "array:open:",
            "string:foo",
            "string:bar",
            "array:close:",
            "member:close:",
            "object:close:",
            // clang-format on
        };
        assert(elements == expected);
    }

    std::string to_string(const tetengo::json::element& element_)
    {
        std::string result{};

        // Obtains the element type name.
        switch (element_.type().name)
        {
        case tetengo::json::element::type_name_type::string:
            result = "string:";
            break;
        case tetengo::json::element::type_name_type::number:
            result = "number:";
            break;
        case tetengo::json::element::type_name_type::boolean:
            result = "boolean:";
            break;
        case tetengo::json::element::type_name_type::null:
            result = "null:";
            break;
        case tetengo::json::element::type_name_type::object:
            result = "object:";
            break;
        case tetengo::json::element::type_name_type::member:
            result = "member:";
            break;
        default:
            assert(element_.type().name == tetengo::json::element::type_name_type::array);
            result = "array:";
            break;
        }

        // Obtains the element type category.
        switch (element_.type().category)
        {
        case tetengo::json::element::type_category_type::primitive:
            break;
        case tetengo::json::element::type_category_type::structure_open:
            result += "open:";
            break;
        default:
            assert(element_.type().category == tetengo::json::element::type_category_type::structure_close);
            result += "close:";
            break;
        }

        // Obtains the element attributes.
        for (const auto& attribute: element_.attributes())
        {
            result += attribute.first + "=" + attribute.second + ":";
        }

        // Obtains the element value.
        result += element_.value();

        return result;
    }
}
// [parsing]
