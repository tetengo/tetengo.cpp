/*! \file
    \brief A train transfer guide.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "timetable_vocabulary.hpp"


namespace
{
    std::unique_ptr<std::istream> create_input_stream(const std::filesystem::path& path)
    {
        auto p_stream = std::make_unique<std::ifstream>(path);
        if (!*p_stream)
        {
            throw std::ios_base::failure{ "Can't open " + path.string() };
        }
        return p_stream;
    }
}


int main(const int argc, char** const argv)
{
    try
    {
        if (argc <= 1)
        {
            std::cout << "Usage: transfer_trains timetable.txt" << std::endl;
            return 0;
        }

        const timetable_vocabulary vocabulary{ create_input_stream(argv[1]) };

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
