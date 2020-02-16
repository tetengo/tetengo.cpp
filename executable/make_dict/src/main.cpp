/*! \file
    \brief A dictionary building tool.

    Copyright (C) 2019-2020 kaoru
 */

#include <cassert>
#include <exception>
#include <fstream> // IWYU pragma: keep
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <boost/format.hpp>


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


}


int main(const int argc, char** const argv)
{
    try
    {
        if (argc <= 1)
        {
            std::cout << "Usage: make_dict UniDic_lex.csv" << std::endl;
            return 0;
        }

        std::ifstream stream{ argv[1] };
        if (!stream)
        {
            throw std::ios_base::failure{ "Can't open the input file." };
        }
        for (auto i = static_cast<std::size_t>(0); stream; ++i)
        {
            std::string line{};
            std::getline(stream, line);
            if (line.empty())
            {
                continue;
            }
            const auto elements = split(line, ',');
            if (elements.size() != 33)
            {
                std::cerr << boost::format{ "%8d: %s" } % i % elements[0] << "    \n" << std::flush;
                throw std::runtime_error{ "Invalid UniDic lex.csv format." };
            }

            if (i % 10000 == 0)
            {
                std::cerr << boost::format{ "%8d: %s" } % i % elements[0] << "    \r" << std::flush;
            }
        }
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
