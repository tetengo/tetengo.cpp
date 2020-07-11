/*! \file
    \brief A train transfer guide.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <cassert>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/format.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include <tetengo/lattice/constraint.hpp>
#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/n_best_iterator.hpp>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/path.hpp>
#include <tetengo/lattice/vocabulary.hpp> // IWYU pragma: keep

#include "timetable.hpp"


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

    std::string to_time_string(const int time_value)
    {
        assert(0 <= time_value && time_value < 1440);
        return (boost::format{ "%02d:%02d" } % (time_value / 60) % (time_value % 60)).str();
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

        const timetable timetable_{ create_input_stream(argv[1]) };

        tetengo::lattice::lattice lattice_{ timetable_.create_vocabulary() };
        for (auto i = static_cast<std::size_t>(0); i + 1 < timetable_.stations().size(); ++i)
        {
            const auto key =
                timetable_.stations()[i].telegram_code() + "-" + timetable_.stations()[i + 1].telegram_code() + "/";
            lattice_.push_back(key);
        }
        const auto eos_and_precedings = lattice_.settle();

        tetengo::lattice::n_best_iterator       iter{ lattice_,
                                                eos_and_precedings.first,
                                                std::make_unique<tetengo::lattice::constraint>() };
        const tetengo::lattice::n_best_iterator last{};
        for (auto i = static_cast<std::size_t>(0); i < 10 && iter != last; ++i, ++iter)
        {
            const auto& path = *iter;
            for (const auto& node: path.nodes())
            {
                const auto* const p_section = std::any_cast<section>(&node.value());
                if (!p_section)
                {
                    continue;
                }
                std::cout << boost::format("    %5s %-16s %5s->%5s %s") % p_section->p_train()->number() %
                                 p_section->p_train()->name() %
                                 to_time_string(static_cast<int>(
                                     *p_section->p_train()->stops()[p_section->from()].departure_time())) %
                                 to_time_string(
                                     static_cast<int>(*p_section->p_train()->stops()[p_section->to()].arrival_time())) %
                                 node.key()
                          << std::endl;
            }
            std::cout << to_time_string(path.cost()) << std::endl;
            ;
            std::cout << "--------" << std::endl;
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
