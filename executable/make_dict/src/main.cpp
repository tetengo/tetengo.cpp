/*! \file
    \brief A dictionary building tool.

    Copyright (C) 2019-2020 kaoru
 */

#include <exception>
#include <iostream>


int main(const int argc, char** const /*argv*/)
{
    try
    {
        if (argc <= 2)
        {
            std::cout << "Usage: make_dict input.txt output.bin" << std::endl;
            return 0;
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
