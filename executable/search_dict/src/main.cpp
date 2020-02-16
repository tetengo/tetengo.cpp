/*! \file
    \brief A double array listing tool.

    Copyright (C) 2019-2020 kaoru
 */

#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream> // IWYU pragma: keep
#include <iostream>
#include <memory>
#include <string_view>
#include <utility>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/storage.hpp>


namespace
{
    std::unique_ptr<tetengo::trie::storage> create_storage(const std::filesystem::path& path_)
    {
        std::ifstream input_stream{ path_.c_str(), std::ios_base::binary };
        return std::make_unique<tetengo::trie::memory_storage>(input_stream, [](const std::string_view& serialized) {
            static const tetengo::trie::default_deserializer<std::int32_t> int32_deserializer{};
            return int32_deserializer(serialized);
        });
    }


}


int main(const int argc, char** const argv)
{
    try
    {
        if (argc <= 1)
        {
            std::cout << "Usage: search_dict input.bin" << std::endl;
            return 0;
        }

        auto                        p_storage = create_storage(argv[1]);
        tetengo::trie::double_array double_array_{ std::move(p_storage), 0 };


        for (const auto& e: double_array_)
        {
            std::cout << e << std::endl;
        }

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
