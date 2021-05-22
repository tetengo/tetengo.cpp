/*! \file
    \brief A json writer.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include "tetengo.property.json_writer.hpp"


namespace tetengo::property
{
    class json_writer::impl : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = json_writer::value_map_type;


        // static functions

        static const json_writer& instance()
        {
            static const json_writer singleton{};
            return singleton;
        }


        // functions

        void write(const value_map_type& value_map, std::ostream& stream) const
        {
            const auto    keys = keys_of(value_map);
            key_tree_type key_tree{};
            std::for_each(std::begin(keys), std::end(keys), [&value_map, &key_tree](const auto& key) {
                add_to_key_tree(key, key, value_map, key_tree);
            });
            print_json(key_tree, value_map, stream);
        }


    private:
        // types

        struct key_tree_type
        {
            using type =
                std::map<std::string, std::variant<std::unique_ptr<key_tree_type>, const value_map_type::mapped_type*>>;

            type m_value;

            key_tree_type() : m_value{} {}

            const type& get() const
            {
                return m_value;
            }

            type& get()
            {
                return m_value;
            }
        };


        // static functions

        static std::vector<std::filesystem::path> keys_of(const value_map_type& value_map)
        {
            std::vector<std::filesystem::path> keys{};
            keys.reserve(std::size(value_map));
            std::transform(std::begin(value_map), std::end(value_map), std::back_inserter(keys), [](const auto& v) {
                return v.first;
            });
            std::sort(std::begin(keys), std::end(keys));
            return keys;
        }

        static void add_to_key_tree(
            const std::filesystem::path& key,
            const std::filesystem::path& full_key,
            const value_map_type&        value_map,
            key_tree_type&               key_tree)
        {
            const auto  top_and_descendant = split(key);
            const auto  top = top_and_descendant.first.string();
            const auto& descendant = top_and_descendant.second;
            if (!std::empty(top_and_descendant.second))
            {
                if (key_tree.get().find(top) == std::end(key_tree.get()))
                {
                    key_tree.get().insert(std::make_pair(top, std::make_unique<key_tree_type>()));
                }
                add_to_key_tree(
                    descendant,
                    full_key,
                    value_map,
                    *std::get<std::unique_ptr<key_tree_type>>(key_tree.get().find(top)->second));
            }
            else
            {
                const auto value_found = value_map.find(full_key.string());
                assert(value_found != std::end(value_map));
                key_tree.get().insert(std::make_pair(top, &value_found->second));
            }
        }

        static std::pair<std::filesystem::path, std::filesystem::path> split(const std::filesystem::path& key)
        {
            const std::vector<std::filesystem::path> elements{ std::begin(key), std::end(key) };
            assert(!std::empty(elements));
            return std::make_pair(
                elements[0],
                std::accumulate(
                    std::next(std::begin(key)),
                    std::end(key),
                    std::filesystem::path{},
                    [](const auto& p1, const auto& p2) { return p1 / p2; }));
        }

        static void print_json(const key_tree_type& key_tree, const value_map_type& value_map, std::ostream& stream)
        {
            stream << "{" << std::endl;
            print_json_iter(key_tree, value_map, stream, 0);
            stream << "}" << std::endl;
        }

        static void print_json_iter(
            const key_tree_type&  key_tree,
            const value_map_type& value_map,
            std::ostream&         stream,
            const std::size_t     level)
        {
            for (auto i = std::begin(key_tree.get()); i != std::end(key_tree.get()); ++i)
            {
                const auto& key_tree_element = *i;
                stream << std::string((level + 1) * 2, ' ') << "\"" << escape(key_tree_element.first) << "\": ";
                if (std::holds_alternative<std::unique_ptr<key_tree_type>>(key_tree_element.second))
                {
                    stream << "{" << std::endl;
                    print_json_iter(
                        *std::get<std::unique_ptr<key_tree_type>>(key_tree_element.second),
                        value_map,
                        stream,
                        level + 1);
                    stream << std::string((level + 1) * 2, ' ') << "}";
                }
                else
                {
                    stream << to_string(*std::get<const value_map_type::mapped_type*>(key_tree_element.second));
                }
                if (std::next(i) != std::end(key_tree.get()))
                {
                    stream << ",";
                }
                stream << std::endl;
            }
        }

        static std::string to_string(const value_map_type::mapped_type& value)
        {
            if (std::holds_alternative<bool>(value))
            {
                return std::get<bool>(value) ? "true" : "false";
            }
            else if (std::holds_alternative<std::uint32_t>(value))
            {
                return std::to_string(std::get<std::uint32_t>(value));
            }
            else
            {
                assert(std::holds_alternative<std::string>(value));
                return "\"" + escape(std::get<std::string>(value)) + "\"";
            }
        }

        static std::string escape(const std::string_view& unescaped)
        {
            std::string escaped{};
            for (const auto u: unescaped)
            {
                switch (u)
                {
                case '"':
                    escaped += "\\\"";
                    break;
                case '\\':
                    escaped += "\\\\";
                    break;
                case '/':
                    escaped += "\\/";
                    break;
                case '\b':
                    escaped += "\\b";
                    break;
                case '\f':
                    escaped += "\\f";
                    break;
                case '\n':
                    escaped += "\\n";
                    break;
                case '\r':
                    escaped += "\\r";
                    break;
                case '\t':
                    escaped += "\\t";
                    break;
                default:
                    escaped += u;
                    break;
                }
            }
            return escaped;
        }
    };


    const json_writer& json_writer::instance()
    {
        return impl::instance();
    }

    json_writer::~json_writer() = default;

    void json_writer::write(const value_map_type& value_map, std::ostream& stream) const
    {
        m_p_impl->write(value_map, stream);
    }

    json_writer::json_writer() : m_p_impl{ std::make_unique<impl>() } {}
}
