/*!
    \brief A double array listing tool.

    Copyright (C) 2019 kaoru
 */

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include <tetengo/trie/storage.hpp>


namespace
{
    std::unique_ptr<tetengo::trie::storage> create_storage(const std::filesystem::path& path_)
    {
        std::ifstream input_stream{ path_.c_str(), std::ios_base::binary };

        return std::make_unique<tetengo::trie::storage>(input_stream);
    }


}


int main(const int argc, char** const argv)
{
    try
    {
        if (argc < 2)
        {
            std::cout << "Usage: list_double_array input.bin" << std::endl;
            return 0;
        }

        const auto p_storate = create_storage(argv[1]);

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
