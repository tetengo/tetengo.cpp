/*! \file
    \brief A dictionary search tool.

    Copyright (C) 2019-2020 kaoru
 */

#include <any>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/trie.hpp>


namespace
{
    std::any deserialize_value(const std::string_view& bytes)
    {
        static const tetengo::trie::default_deserializer<std::size_t> size_t_deserializer{};

        const auto size = size_t_deserializer(bytes.substr(0, sizeof(std::size_t)));
        std::vector<std::pair<std::size_t, std::size_t>> value{};
        value.reserve(size);

        for (auto i = static_cast<std::size_t>(0); i < size; ++i)
        {
            const auto byte_offset = sizeof(std::size_t) * (i * 2 + 1);
            const auto offset = size_t_deserializer(bytes.substr(byte_offset, sizeof(std::size_t)));
            const auto length =
                size_t_deserializer(bytes.substr(byte_offset + sizeof(std::size_t), sizeof(std::size_t)));
            value.emplace_back(offset, length);
        }

        return value;
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
