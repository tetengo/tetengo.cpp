/*! \file
    \brief A dictionary building tool.

    Copyright (C) 2019-2020 kaoru
 */

#include <algorithm>
#include <any>
#include <cassert>
#include <cstdint>
#include <exception>
#include <fstream> // IWYU pragma: keep
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/format.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/trie.hpp>


namespace
{
    std::vector<std::string_view> split(const std::string_view& string, const char delimiter)
    {
        std::vector<std::string_view> elements{};

        auto first = static_cast<std::size_t>(0);
        for (;;)
        {
            if (first < string.length() && string[first] == '"')
            {
                const auto last = string.find_first_of('"', first + 1);
                if (last == std::string_view::npos)
                {
                    elements.push_back(string.substr(first + 1));
                    break;
                }
                elements.push_back(string.substr(first + 1, last - first - 1));
                assert(string[last + 1] == delimiter);
                first = last + 2;
            }
            else
            {
                const auto last = string.find_first_of(delimiter, first);
                if (last == std::string_view::npos)
                {
                    elements.push_back(string.substr(first));
                    break;
                }
                elements.push_back(string.substr(first, last - first));
                first = last + 1;
            }
        }

        return elements;
    }

    void insert_word_offset_to_map(
        const std::string_view&                                                            key,
        const std::size_t                                                                  offset,
        const std::size_t                                                                  length,
        std::unordered_map<std::string, std::vector<std::pair<std::size_t, std::size_t>>>& map)
    {
        auto i_value = map.find(std::string{ key });
        if (i_value == map.end())
        {
            i_value = map.insert(std::make_pair(key, std::vector<std::pair<std::size_t, std::size_t>>{})).first;
        }
        assert(i_value != map.end());

        if (std::find(i_value->second.begin(), i_value->second.end(), std::make_pair(offset, length)) !=
            i_value->second.end())
        {
            return;
        }
        i_value->second.emplace_back(offset, length);
    }

    class trie_building_observer
    {
    public:
        trie_building_observer() : m_index{ 0 } {}

        void operator()(const std::string_view& key)
        {
            if (m_index % 10000 == 0)
            {
                std::cerr << boost::format{ "%8d: %s" } % m_index % key << "    \r" << std::flush;
            }
            ++m_index;
        }

    private:
        std::size_t m_index;
    };

    std::string serialize_size_t(const std::size_t s)
    {
        assert(s <= std::numeric_limits<std::uint32_t>::max());

        std::string serialized(sizeof(std::uint32_t), '\0');

        for (auto i = static_cast<std::size_t>(0); i < sizeof(std::uint32_t); ++i)
        {
            serialized[i] = (s >> ((sizeof(std::uint32_t) - i - 1) * 8)) & 0xFF;
        }

        return serialized;
    }

    std::string serialize_pair_of_size_t(const std::pair<std::size_t, std::size_t>& ps)
    {
        std::string serialized{};
        serialized.reserve(sizeof(std::uint32_t) * 2);

        serialized += serialize_size_t(ps.first);
        serialized += serialize_size_t(ps.second);

        return serialized;
    }

    std::string serialize_vector_of_pair_of_size_t(const std::vector<std::pair<std::size_t, std::size_t>>& vps)
    {
        std::string serialized{};
        serialized.reserve(sizeof(std::uint32_t) * (1 + 2 * vps.size()));

        serialized += serialize_size_t(vps.size());
        for (const auto& ps: vps)
        {
            serialized += serialize_pair_of_size_t(ps);
        }

        return serialized;
    }

    std::string serialize_value(const std::any& value)
    {
        const auto* const p_value = std::any_cast<std::vector<std::pair<std::size_t, std::size_t>>>(&value);
        assert(p_value);
        return serialize_vector_of_pair_of_size_t(*p_value);
    }


}


int main(const int argc, char** const argv)
{
    try
    {
        if (argc <= 2)
        {
            std::cout << "Usage: make_dict UniDic_lex.csv trie.bin" << std::endl;
            return 0;
        }

        std::ifstream stream{ argv[1] };
        if (!stream)
        {
            throw std::ios_base::failure{ "Can't open the input file." };
        }

        std::unordered_map<std::string, std::vector<std::pair<std::size_t, std::size_t>>> word_offset_map{};

        std::cerr << "Loading UniDic lex.csv..." << std::endl;
        auto line_head = static_cast<std::size_t>(0);
        for (auto i = static_cast<std::size_t>(0); stream; ++i)
        {
            std::string line{};
            std::getline(stream, line);
            if (line.empty())
            {
                line_head = line.length() + 1;
                continue;
            }
            const auto elements = split(line, ',');
            if (elements.size() != 33)
            {
                std::cerr << boost::format{ "%8d: %s" } % i % elements[0] << "    \n" << std::flush;
                throw std::runtime_error{ "Invalid UniDic lex.csv format." };
            }

            insert_word_offset_to_map(elements[12], line_head, line.length() + 1, word_offset_map);
            insert_word_offset_to_map(elements[24], line_head, line.length() + 1, word_offset_map);

            if (i % 10000 == 0)
            {
                std::cerr << boost::format{ "%8d: %s" } % i % elements[0] << "    \r" << std::flush;
            }

            line_head = line.length() + 1;
        }
        std::cerr << "Done.        " << std::endl;

        std::cerr << "Building trie..." << std::endl;
        const tetengo::trie::trie<std::string_view, std::vector<std::pair<std::size_t, std::size_t>>> trie_{
            std::make_move_iterator(std::begin(word_offset_map)),
            std::make_move_iterator(std::end(word_offset_map)),
            tetengo::trie::default_serializer<std::string_view>{},
            tetengo::trie::trie<std::string_view, std::vector<std::pair<std::size_t, std::size_t>>>::
                building_observer_set_type{ trie_building_observer{}, []() {} }
        };
        std::cerr << "Done.        " << std::endl;

        std::cerr << "Serializing trie..." << std::endl;
        std::ofstream output_stream{ argv[2], std::ios_base::binary };
        if (!output_stream)
        {
            throw std::ios_base::failure{ "Can't open the output file." };
        }
        trie_.get_storage().serialize(output_stream, serialize_value);
        std::cerr << "Done.        " << std::endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Error: unknown error." << std::endl;
        return 1;
    }
}
