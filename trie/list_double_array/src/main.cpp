/*!
    \brief A double array listing tool.

    Copyright (C) 2019 kaoru
 */

#include <exception>
#include <filesystem>
#include <fstream> // IWYU pragma: keep
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/enumerator.hpp>
#include <tetengo/trie/memory_storage.hpp>


namespace
{
    std::unique_ptr<tetengo::trie::memory_storage> create_storage(const std::filesystem::path& path_)
    {
        std::ifstream input_stream{ path_.c_str(), std::ios_base::binary };
        return std::make_unique<tetengo::trie::memory_storage>(input_stream);
    }


}


int main(const int argc, char** const argv)
{
    try
    {
        if (argc <= 1)
        {
            std::cout << "Usage: list_double_array input.bin" << std::endl;
            return 0;
        }

        auto                        p_storage = create_storage(argv[1]);
        tetengo::trie::double_array double_array_{ std::move(p_storage) };


        for (auto enumerator = double_array_.get_enumerator();;)
        {
            auto o_element = enumerator.next();
            if (!o_element)
            {
                break;
            }

            std::cout << o_element->first << "\t" << o_element->second << std::endl;
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
