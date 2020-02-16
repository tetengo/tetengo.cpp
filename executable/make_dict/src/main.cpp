/*! \file
    \brief A double array building tool.

    Copyright (C) 2019 kaoru
 */

#include <algorithm>
#include <any>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream> // IWYU pragma: keep
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/format.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/storage.hpp>


namespace
{
    std::unique_ptr<std::vector<std::pair<std::string, int>>> load_input(const std::filesystem::path& path)
    {
        auto p_input = std::make_unique<std::vector<std::pair<std::string, int>>>();

        std::ifstream input_stream{ path.c_str() };
        for (auto i = 0; input_stream.good(); ++i)
        {
            std::string line;
            std::getline(input_stream, line);

            const auto  split_position = std::find(std::begin(line), std::end(line), '\t');
            std::string key{ std::begin(line), split_position };
            p_input->emplace_back(std::move(key), i);
        }

        return p_input;
    }

    class adding_observer_type
    {
    public:
        explicit adding_observer_type(const std::size_t size) : m_size{ size }, m_count{ 0 } {}

        void operator()(const std::pair<std::string_view, std::int32_t>& element)
        {
            if (m_count % 5000 == 0)
            {
                const auto percentage = (100 * m_count / m_size) * (100 * m_count / m_size) / 100;
                std::cout << boost::format{ "%8d/%8d (%3d%%) %s    \r" } % m_count % m_size % percentage % element.first
                          << std::flush;
            }
            ++m_count;
        }

    private:
        const std::size_t m_size;
        std::size_t       m_count;
    };

    class done_observer_type
    {
    public:
        void operator()() const
        {
            std::cout << "done.                " << std::endl;
        }
    };

    void save_double_array(const tetengo::trie::double_array& double_array_, const std::filesystem::path& path)
    {
        std::ofstream output_stream{ path.c_str(), std::ios_base::binary };
        if (!output_stream.is_open())
        {
            throw std::ios_base::failure{ "Can't open the output file." };
        }

        double_array_.get_storage().serialize(output_stream, [](const std::any& object) {
            static const tetengo::trie::default_serializer<int> int_serializer{};
            return int_serializer(std::any_cast<int>(object));
        });
    }


}


int main(const int argc, char** const argv)
{
    try
    {
        if (argc <= 2)
        {
            std::cout << "Usage: make_dict input.txt output.bin" << std::endl;
            return 0;
        }

        std::cerr << "Loading input file..." << std::flush;
        const auto p_input = load_input(argv[1]);
        std::cerr << "done." << std::endl;

        std::cout << "Building double array..." << std::endl;
        const tetengo::trie::double_array double_array_{
            p_input->begin(), p_input->end(), { adding_observer_type{ p_input->size() }, done_observer_type{} }
        };
        std::cerr << boost::format{ "Filling rate: %3.1f%%" } % (double_array_.get_storage().filling_rate() * 100.0)
                  << std::endl;

        std::cerr << "Writing double array to file..." << std::flush;
        save_double_array(double_array_, argv[2]);
        std::cerr << "done." << std::endl;

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
