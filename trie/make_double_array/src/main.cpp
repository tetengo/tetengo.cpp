/*!
    \brief A double array build tool.

    Copyright (C) 2019 kaoru
 */

#include <fstream>
#include <iostream>
#include <string>


int main(const int argc, char** const argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: make_double_array input.txt output.bin" << std::endl;
        return 0;
    }

    {
        std::ifstream input_stream{ argv[1] };
        while (input_stream.good())
        {
            std::string line;
            std::getline(input_stream, line);

            std::cout << line << std::endl;
        }
    }

    return 0;
}
