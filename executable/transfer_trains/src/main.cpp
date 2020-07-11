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
#include <type_traits>
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

    std::size_t get_station_index(const std::string& prompt, const timetable& timetable_)
    {
        std::cout << prompt << ": ";
        std::string input{};
        std::cin >> input;

        return timetable_.station_index(input);
    }

    std::optional<std::pair<std::size_t, std::size_t>> get_departure_and_arrival(const timetable& timetable_)
    {
        const auto departure_station_index = get_station_index("Departure", timetable_);
        if (departure_station_index >= timetable_.stations().size())
        {
            std::cout << "No departure station is found." << std::endl;
            return std::nullopt;
        }
        const auto arrival_station_index = get_station_index("Arrival", timetable_);
        if (arrival_station_index >= timetable_.stations().size())
        {
            std::cout << "No arrival station is found." << std::endl;
            return std::nullopt;
        }
        return std::make_pair(departure_station_index, arrival_station_index);
    }

    void build_lattice(
        const std::pair<std::size_t, std::size_t>& departure_and_arrival,
        const timetable&                           timetable_,
        tetengo::lattice::lattice&                 lattice_)
    {
        for (auto i = departure_and_arrival.first; i < departure_and_arrival.second; ++i)
        {
            const auto key =
                timetable_.stations()[i].telegram_code() + "-" + timetable_.stations()[i + 1].telegram_code() + "/";
            lattice_.push_back(key);
        }
    }

    std::string to_time_string(const int time_value)
    {
        assert(0 <= time_value && time_value < 1440);
        return (boost::format{ "%02d:%02d" } % (time_value / 60) % (time_value % 60)).str();
    }

    void enumerate_paths(
        const tetengo::lattice::lattice&                                            lattice_,
        const std::pair<tetengo::lattice::node, std::unique_ptr<std::vector<int>>>& eos_and_precedings)
    {
        tetengo::lattice::n_best_iterator       iter{ lattice_,
                                                eos_and_precedings.first,
                                                std::make_unique<tetengo::lattice::constraint>() };
        const tetengo::lattice::n_best_iterator last{};
        for (auto i = static_cast<std::size_t>(0); i < 20 && iter != last; ++i, ++iter)
        {
            const auto& path = *iter;
            if (path.cost() >= 1440)
            {
                continue;
            }
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

        while (std::cin)
        {
            const auto departure_and_arrival = get_departure_and_arrival(timetable_);
            if (!departure_and_arrival)
            {
                continue;
            }

            tetengo::lattice::lattice lattice_{ timetable_.create_vocabulary() };
            build_lattice(*departure_and_arrival, timetable_, lattice_);
            const auto eos_and_precedings = lattice_.settle();

            enumerate_paths(lattice_, eos_and_precedings);
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
