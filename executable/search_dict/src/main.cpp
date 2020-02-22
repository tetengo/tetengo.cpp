/*! \file
    \brief A dictionary search tool.

    Copyright (C) 2019-2020 kaoru
 */

#include <any>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream> // IWYU pragma: keep
#include <iostream>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/trie.hpp>


namespace
{
    std::size_t deserialize_size_t(const std::string_view& bytes, std::size_t& byte_offset)
    {
        auto value = static_cast<std::size_t>(0);
        for (auto i = static_cast<std::size_t>(0); i < sizeof(std::uint32_t); ++i)
        {
            value <<= 8;
            value |= static_cast<unsigned char>(bytes[byte_offset + i]);
        }
        byte_offset += sizeof(std::uint32_t);
        return value;
    }

    std::pair<std::size_t, std::size_t>
    deserialize_pair_of_size_t(const std::string_view& bytes, std::size_t& byte_offset)
    {
        const auto first = deserialize_size_t(bytes, byte_offset);
        const auto second = deserialize_size_t(bytes, byte_offset);
        return std::make_pair(first, second);
    }

    std::vector<std::pair<std::size_t, std::size_t>>
    deserialize_vector_of_pair_of_size_t(const std::string_view& bytes, std::size_t& byte_offset)
    {
        std::vector<std::pair<std::size_t, std::size_t>> vps;

        const auto size = deserialize_size_t(bytes, byte_offset);
        vps.reserve(size);
        for (auto i = static_cast<std::size_t>(0); i < size; ++i)
        {
            vps.push_back(deserialize_pair_of_size_t(bytes, byte_offset));
        }

        return vps;
    }

    std::any deserialize_value(const std::string_view& bytes)
    {
        auto byte_offset = static_cast<std::size_t>(0);
        return deserialize_vector_of_pair_of_size_t(bytes, byte_offset);
    }

    std::unique_ptr<tetengo::trie::trie<std::string_view, std::vector<std::pair<std::size_t, std::size_t>>>>
    load_trie(const std::filesystem::path& trie_path)
    {
        std::ifstream stream{ trie_path, std::ios_base::binary };
        if (!stream)
        {
            throw std::ios_base::failure{ "Can't open the trie.bin file." };
        }

        auto p_storage = std::make_unique<tetengo::trie::memory_storage>(stream, deserialize_value);
        auto p_trie =
            std::make_unique<tetengo::trie::trie<std::string_view, std::vector<std::pair<std::size_t, std::size_t>>>>(
                std::move(p_storage));
        return p_trie;
    }


}


int main(const int argc, char** const argv)
{
    try
    {
        if (argc <= 1)
        {
            std::cout << "Usage: search_dict trie.bin" << std::endl;
            return 0;
        }

        const auto p_trie = load_trie(argv[1]);

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
