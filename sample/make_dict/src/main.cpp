/*! \file
    \brief A dictionary building tool.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <any>
#include <cassert>
#include <clocale>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream> // IWYU pragma: keep
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <locale>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits> // IWYU pragma: keep
#include <unordered_map>
#include <utility>
#include <vector>

#include <tetengo/text/encoder.hpp>
#include <tetengo/text/encoding/cp932.hpp> // IWYU pragma: keep
#include <tetengo/text/encoding/utf8.hpp> // IWYU pragma: keep
#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/value_serializer.hpp>


namespace
{
    constexpr auto value_capacity = static_cast<std::size_t>(4);

    constexpr auto serialized_value_size = sizeof(std::uint32_t) * (1 + 4 * 2);

    std::string encode_for_print(const std::string_view& string_)
    {
        const char* const locale_name = std::setlocale(LC_CTYPE, nullptr);
        if (locale_name && std::string_view{ locale_name } == "Japanese_Japan.932")
        {
            return tetengo::text::encoder<tetengo::text::encoding::cp932>::instance().encode(string_);
        }
        else
        {
            return std::string{ tetengo::text::encoder<tetengo::text::encoding::utf8>::instance().encode(string_) };
        }
    }

    std::vector<std::string_view> split(const std::string_view& string, const char delimiter)
    {
        std::vector<std::string_view> elements{};

        for (auto first = static_cast<std::size_t>(0);;)
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
        if (i_value == std::end(map))
        {
            i_value = map.insert(std::make_pair(key, std::vector<std::pair<std::size_t, std::size_t>>{})).first;
        }
        assert(i_value != std::end(map));

        if (std::find(std::begin(i_value->second), std::end(i_value->second), std::make_pair(offset, length)) !=
            std::end(i_value->second))
        {
            return;
        }
        if (std::size(i_value->second) < value_capacity)
        {
            i_value->second.emplace_back(offset, length);
        }
        else
        {
            i_value->second.emplace_back(0, 0);
        }
    }

    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    const std::string string_kigo{ 0xE8_c, 0xA8_c, 0x98_c, 0xE5_c, 0x8F_c, 0xB7_c }; // "kigo" in Kanji in UTF-8

    const std::string string_hojo{ 0xE8_c, 0xA3_c, 0x9C_c, 0xE5_c, 0x8A_c, 0xA9_c }; // "hojo" in Kanji in UTF-8

    std::unordered_map<std::string, std::vector<std::pair<std::size_t, std::size_t>>>
    load_lex_csv(const std::filesystem::path& lex_csv_path)
    {
        std::ifstream stream{ lex_csv_path };
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
            if (std::empty(line))
            {
                line_head += line.length() + 1;
                continue;
            }
            const auto elements = split(line, ',');
            if (std::size(elements) != 33)
            {
                std::cerr << std::format("{:8d}: {:s}    \n", i, encode_for_print(elements[0])) << std::flush;
                throw std::runtime_error{ "Invalid UniDic lex.csv format." };
            }

            if (elements[16] == string_kigo && elements[23] == string_hojo)
            {
                insert_word_offset_to_map(elements[0], line_head, line.length() + 1, word_offset_map);
            }
            else
            {
                insert_word_offset_to_map(elements[12], line_head, line.length() + 1, word_offset_map);
                insert_word_offset_to_map(elements[24], line_head, line.length() + 1, word_offset_map);
            }

            if (i % 10000 == 0)
            {
                std::cerr << std::format("{:8d}: {:s}    \r", i, encode_for_print(elements[0])) << std::flush;
            }

            line_head += line.length() + 1;
        }
        std::cerr << "Done.        " << std::endl;

        return word_offset_map;
    }

    class trie_building_observer
    {
    public:
        trie_building_observer() : m_index{ 0 } {}

        void operator()(const std::string_view& key)
        {
            if (m_index % 10000 == 0)
            {
                std::cerr << std::format("{:8d}: {:s}    \r", m_index, encode_for_print(key)) << std::flush;
            }
            ++m_index;
        }

    private:
        std::size_t m_index;
    };

    std::unique_ptr<tetengo::trie::trie<std::string_view, std::vector<std::pair<std::size_t, std::size_t>>>>
    build_trie(const std::unordered_map<std::string, std::vector<std::pair<std::size_t, std::size_t>>>& word_offset_map)
    {
        std::cerr << "Building trie..." << std::endl;
        auto p_trie =
            std::make_unique<tetengo::trie::trie<std::string_view, std::vector<std::pair<std::size_t, std::size_t>>>>(
                std::make_move_iterator(std::begin(word_offset_map)),
                std::make_move_iterator(std::end(word_offset_map)),
                tetengo::trie::default_serializer<std::string_view>{ true },
                tetengo::trie::trie<std::string_view, std::vector<std::pair<std::size_t, std::size_t>>>::
                    building_observer_set_type{ trie_building_observer{}, []() {} });
        std::cerr << "Done.        " << std::endl;
        return p_trie;
    }

    std::vector<char> serialize_size_t(const std::size_t s)
    {
        assert(s <= std::numeric_limits<std::uint32_t>::max());

        std::vector<char> serialized(sizeof(std::uint32_t), '\0');

        for (auto i = static_cast<std::size_t>(0); i < sizeof(std::uint32_t); ++i)
        {
            serialized[i] = (s >> ((sizeof(std::uint32_t) - i - 1) * 8)) & 0xFF;
        }

        return serialized;
    }

    std::vector<char> serialize_pair_of_size_t(const std::pair<std::size_t, std::size_t>& ps)
    {
        std::vector<char> serialized{};
        serialized.reserve(sizeof(std::uint32_t) * 2);

        const auto serialized_offset = serialize_size_t(ps.first);
        serialized.insert(std::end(serialized), std::begin(serialized_offset), std::end(serialized_offset));
        const auto serialized_length = serialize_size_t(ps.second);
        serialized.insert(std::end(serialized), std::begin(serialized_length), std::end(serialized_length));

        return serialized;
    }

    std::vector<char> serialize_vector_of_pair_of_size_t(const std::vector<std::pair<std::size_t, std::size_t>>& vps)
    {
        std::vector<char> serialized{};
        serialized.reserve(serialized_value_size);

        const auto serialized_size = serialize_size_t(std::size(vps));
        serialized.insert(std::end(serialized), std::begin(serialized_size), std::end(serialized_size));
        for (auto i = static_cast<std::size_t>(0); i < value_capacity; ++i)
        {
            if (i < std::size(vps))
            {
                const auto& ps = vps[i];
                const auto  serialized_element = serialize_pair_of_size_t(ps);
                serialized.insert(std::end(serialized), std::begin(serialized_element), std::end(serialized_element));
            }
            else
            {
                const auto serialized_element =
                    serialize_pair_of_size_t(std::make_pair<std::size_t, std::size_t>(0, 0));
                serialized.insert(std::end(serialized), std::begin(serialized_element), std::end(serialized_element));
            }
        }

        return serialized;
    }

    std::vector<char> serialize_value(const std::any& value)
    {
        const auto* const p_value = std::any_cast<std::vector<std::pair<std::size_t, std::size_t>>>(&value);
        assert(p_value);
        return serialize_vector_of_pair_of_size_t(*p_value);
    }

    void serialize_trie(
        const tetengo::trie::trie<std::string_view, std::vector<std::pair<std::size_t, std::size_t>>>& trie_,
        const std::filesystem::path&                                                                   trie_bin_path)
    {
        std::cerr << "Serializing trie..." << std::endl;
        std::ofstream output_stream{ trie_bin_path, std::ios_base::binary };
        if (!output_stream)
        {
            throw std::ios_base::failure{ "Can't open the output file." };
        }
        const tetengo::trie::value_serializer serializer{ serialize_value, serialized_value_size };
        trie_.get_storage().serialize(output_stream, serializer);
        std::cerr << "Done.        " << std::endl;
    }

}


int main(const int argc, char** const argv)
{
    try
    {
        std::locale::global(std::locale{ "" });

        if (argc <= 2)
        {
            std::cerr << "Usage: make_dict UniDic_lex.csv trie.bin" << std::endl;
            return 0;
        }

        const auto word_offset_map = load_lex_csv(argv[1]);
        const auto p_trie = build_trie(word_offset_map);
        serialize_trie(*p_trie, argv[2]);

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
