/*! \file
    \brief A train transfer guide.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <cassert>
#include <cstddef> // IWYU pragma: keep
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/stl_interfaces/iterator_interface.hpp>

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

    std::size_t get_time(const std::string& prompt)
    {
        std::cout << prompt << ": ";
        std::string input{};
        std::cin >> input;

        std::vector<std::string> elements{};
        boost::split(elements, input, boost::is_any_of(":"));
        if (std::size(elements) != 2)
        {
            return 1440;
        }
        auto hour = static_cast<std::size_t>(0);
        auto minute = static_cast<std::size_t>(0);
        try
        {
            hour = boost::lexical_cast<std::size_t>(elements[0]);
            minute = boost::lexical_cast<std::size_t>(elements[1]);
        }
        catch (const boost::bad_lexical_cast&)
        {
            return 1440;
        }
        if (hour >= 24 || minute >= 60)
        {
            return 1440;
        }
        return hour * 60 + minute;
    }

    std::optional<std::pair<std::pair<std::size_t, std::size_t>, std::size_t>>
    get_departure_and_arrival(const timetable& timetable_)
    {
        const auto departure_station_index = get_station_index("Departure Station", timetable_);
        if (departure_station_index >= std::size(timetable_.stations()))
        {
            std::cout << "No departure station is found." << std::endl;
            return std::nullopt;
        }

        const auto departure_time = get_time("Departure Time");
        if (departure_time >= 1440)
        {
            std::cout << "Wrong time format." << std::endl;
            return std::nullopt;
        }

        const auto arrival_station_index = get_station_index("Arrival Station", timetable_);
        if (arrival_station_index >= std::size(timetable_.stations()))
        {
            std::cout << "No arrival station is found." << std::endl;
            return std::nullopt;
        }
        return std::make_pair(std::make_pair(departure_station_index, departure_time), arrival_station_index);
    }

    void build_lattice(
        const std::pair<std::pair<std::size_t, std::size_t>, std::size_t>& departure_and_arrival,
        const timetable&                                                   timetable_,
        tetengo::lattice::lattice&                                         lattice_)
    {
        for (auto i = departure_and_arrival.first.first; i < departure_and_arrival.second; ++i)
        {
            const auto key =
                timetable_.stations()[i].telegram_code() + "-" + timetable_.stations()[i + 1].telegram_code() + "/";
            lattice_.push_back(key);
        }
    }

    struct trip_section
    {
        std::string_view train_number;

        std::string_view train_name;

        std::size_t departure_time;

        std::size_t departure_station;

        std::size_t arrival_time;

        std::size_t arrival_station;
    };

    struct trip
    {
        std::vector<trip_section> sections;

        int cost;
    };

    std::vector<trip> enumerate_trips(
        const tetengo::lattice::lattice&                                            lattice_,
        const std::pair<tetengo::lattice::node, std::unique_ptr<std::vector<int>>>& eos_and_precedings,
        const std::size_t                                                           trip_capacity)
    {
        tetengo::lattice::n_best_iterator       iter{ lattice_,
                                                eos_and_precedings.first,
                                                std::make_unique<tetengo::lattice::constraint>() };
        const tetengo::lattice::n_best_iterator last{};
        std::vector<trip>                       trips{};
        trips.reserve(trip_capacity);
        std::unordered_set<std::string_view> duplication_checker{};
        for (; std::size(trips) < trip_capacity && iter != last; ++iter)
        {
            const auto& path = *iter;
            if (path.cost() >= 1440)
            {
                break;
            }

            trip trip_{};
            for (const auto& node: path.nodes())
            {
                const auto* const p_section = std::any_cast<section>(&node.value());
                if (!p_section)
                {
                    continue;
                }

                if (std::empty(trip_.sections) || trip_.sections.back().train_number != p_section->p_train()->number())
                {
                    trip_.sections.push_back({ p_section->p_train()->number(),
                                               p_section->p_train()->name(),
                                               *p_section->p_train()->stops()[p_section->from()].departure_time(),
                                               p_section->from(),
                                               *p_section->p_train()->stops()[p_section->to()].arrival_time(),
                                               p_section->to() });
                }
                else
                {
                    trip_.sections.back().arrival_time = *p_section->p_train()->stops()[p_section->to()].arrival_time();
                    trip_.sections.back().arrival_station = p_section->to();
                }
            }
            trip_.cost = path.cost();

            if (duplication_checker.find(trip_.sections.front().train_number) != std::end(duplication_checker) ||
                duplication_checker.find(trip_.sections.back().train_number) != std::end(duplication_checker))
            {
                continue;
            }

            duplication_checker.insert(trip_.sections.front().train_number);
            duplication_checker.insert(trip_.sections.back().train_number);
            trips.push_back(std::move(trip_));
        }

        return trips;
    }

    std::string to_time_string(const int time_value)
    {
        assert(0 <= time_value && time_value < 1440);
        return (boost::format{ "%02d:%02d" } % (time_value / 60) % (time_value % 60)).str();
    }

    void print_trips(const std::vector<trip>& trips, const timetable& timetable_)
    {
        for (auto i = static_cast<std::size_t>(0); i < std::size(trips); ++i)
        {
            const auto& trip_ = trips[i];

            std::cout << boost::format("[%d] Cost: %d") % (i + 1) % trip_.cost << std::endl;

            for (const auto& section: trip_.sections)
            {
                std::cout << boost::format("    %5s %-40s %5s->%5s %s->%s") % section.train_number %
                                 section.train_name % to_time_string(static_cast<int>(section.departure_time)) %
                                 to_time_string(static_cast<int>(section.arrival_time)) %
                                 timetable_.stations()[section.departure_station].name() %
                                 timetable_.stations()[section.arrival_station].name()
                          << std::endl;
            }
        }

        std::cout << "--------------------------------" << std::endl;
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

            tetengo::lattice::lattice lattice_{ timetable_.create_vocabulary(departure_and_arrival->first.second) };
            build_lattice(*departure_and_arrival, timetable_, lattice_);
            const auto eos_and_precedings = lattice_.settle();

            const auto trips = enumerate_trips(lattice_, eos_and_precedings, 5);

            print_trips(trips, timetable_);
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
