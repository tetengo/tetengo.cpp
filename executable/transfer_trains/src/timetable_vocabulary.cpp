/*! \file
    \brief A timetable vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <algorithm>
#include <cassert>
#include <istream>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/lexical_cast.hpp>

#include "timetable_vocabulary.hpp"


namespace
{
    struct station
    {
        std::string name;

        std::string telegram_code;

        station(std::string&& name, std::string&& telegram_code) :
        name{ std::move(name) },
            telegram_code{ std::move(telegram_code) }
        {}
    };

    struct ad_time
    {
        std::optional<std::size_t> arrival;

        std::optional<std::size_t> departure;

        ad_time(std::optional<std::size_t>&& arrival, std::optional<std::size_t>&& departure) :
        arrival{ std::move(arrival) },
            departure{ std::move(departure) }
        {}
    };

    struct train
    {
        std::string number;

        std::string name;

        std::vector<ad_time> ad_times;

        train(std::string&& number, std::string&& name, std::vector<ad_time>&& ad_times) :
        number{ std::move(number) },
            name{ std::move(name) },
            ad_times{ std::move(ad_times) }
        {}
    };

    struct timetable
    {
        std::vector<station> stations;

        std::vector<train> trains;

        timetable(std::vector<station>&& stations, std::vector<train>&& trains) :
        stations{ std::move(stations) },
            trains{ std::move(trains) }
        {}
    };


}


class timetable_vocabulary::impl : private boost::noncopyable
{
public:
    // constructors and destructor

    explicit impl(std::unique_ptr<std::istream>&& p_input_stream)
    {
        assert(p_input_stream);
        auto timetable = parse_input(*p_input_stream);
        guess_arrival_times(timetable);
    }


private:
    // static functions

    static timetable parse_input(std::istream& input_stream)
    {
        if (!input_stream)
        {
            throw std::runtime_error{ "Input file format error: Empty data." };
        }

        std::vector<station> stations{};
        {
            auto line1 = read_line(input_stream);
            auto line2 = read_line(input_stream);
            stations = parse_stations(std::move(line1), std::move(line2));
        }

        std::vector<train> trains{};
        while (input_stream)
        {
            trains.push_back(parse_train(read_line(input_stream), stations.size()));
        }

        return timetable{ std::move(stations), std::move(trains) };
    }

    static std::vector<std::string> read_line(std::istream& input_stream)
    {
        std::string line;
        std::getline(input_stream, line);

        std::vector<std::string> elements;
        boost::algorithm::split(elements, std::move(line), boost::is_any_of(","));
        if (elements.empty())
        {
            throw std::runtime_error{ "Input file format error: Empty line found." };
        }
        std::for_each(
            std::begin(elements), std::end(elements), [](auto& element) { return boost::algorithm::trim(element); });
        return elements;
    }

    static std::vector<station> parse_stations(std::vector<std::string>&& line1, std::vector<std::string>&& line2)
    {
        line1.erase(std::begin(line1), std::next(std::begin(line1), 2));
        line2.erase(std::begin(line2), std::next(std::begin(line2), 2));
        if (line1.size() != line2.size())
        {
            throw std::runtime_error{ "Input file format error: Station names and telegram codes unmatch." };
        }
        std::vector<station> stations{};
        stations.reserve(line1.size());
        for (auto i = static_cast<std::size_t>(0); i < line1.size(); ++i)
        {
            stations.emplace_back(std::move(line1[i]), std::move(line2[i]));
        }
        return stations;
    }

    static train parse_train(std::vector<std::string>&& line, const std::size_t station_count)
    {
        if (line.size() > station_count + 2)
        {
            throw std::runtime_error{ "Input file format error: Invalid train line found." };
        }
        line.resize(station_count + 2);

        std::vector<ad_time> ad_times{};
        ad_times.reserve(station_count);
        std::transform(std::next(std::begin(line), 2), std::end(line), std::back_inserter(ad_times), [](auto&& e) {
            return to_ad_time(std::move(e));
        });
        return train{ std::move(line[0]), std::move(line[1]), std::move(ad_times) };
    }

    static ad_time to_ad_time(std::string&& element)
    {
        std::vector<std::string> string_times{};
        boost::algorithm::split(string_times, std::move(element), boost::is_any_of("/"));
        std::for_each(std::begin(string_times), std::end(string_times), [](auto&& e) { return boost::trim(e); });
        if (string_times.size() == 0 || string_times.size() > 2)
        {
            throw std::runtime_error{ "Input file format error: Invalid arrival/depature time found." };
        }
        else if (string_times.size() == 1)
        {
            return ad_time{ std::nullopt, to_minutes(std::move(string_times[0])) };
        }
        else
        {
            assert(string_times.size() == 2);
            return ad_time{ to_minutes(std::move(string_times[0])), to_minutes(std::move(string_times[1])) };
        }
    }

    static std::optional<std::size_t> to_minutes(std::string&& string_time)
    {
        if (string_time.empty() || string_time == "-")
        {
            return std::nullopt;
        }

        auto int_time = static_cast<std::size_t>(0);
        try
        {
            int_time = boost::lexical_cast<std::size_t>(string_time);
        }
        catch (const boost::bad_lexical_cast&)
        {
            throw std::runtime_error{ "Input file format error: Invalid time found." };
        }

        const auto hour = int_time / 100;
        const auto minute = int_time - hour * 100;
        if (hour >= 24 || minute >= 60)
        {
            throw std::runtime_error{ "Input file format error: Invalid time found." };
        }

        return hour * 60 + minute;
    }

    static void guess_arrival_times(timetable& timetable_)
    {
        for (auto from = static_cast<std::size_t>(0); from < timetable_.stations.size() - 1; ++from)
        {
            for (auto to = from + 1; to < timetable_.stations.size(); ++to)
            {
                /*const auto minimum_duration_ =*/minimum_duration(timetable_.trains, from, to);
            }
        }
    }

    static std::size_t minimum_duration(const std::vector<train>& trains, const std::size_t from, const std::size_t to)
    {
        auto minimum = std::numeric_limits<std::size_t>::max();
        for (const auto& train: trains)
        {
            if (!all_passing(train.ad_times, from, to))
            {
                continue;
            }
            if (!train.ad_times[from].arrival && !train.ad_times[from].departure)
            {
                continue;
            }
            if (!train.ad_times[to].arrival && !train.ad_times[to].departure)
            {
                continue;
            }

            const auto from_time =
                train.ad_times[from].departure ? *train.ad_times[from].departure : *train.ad_times[from].arrival;
            const auto to_time =
                train.ad_times[to].arrival ? *train.ad_times[to].arrival : *train.ad_times[to].departure;

            if (to_time - from_time < minimum)
            {
                minimum = to_time - from_time;
            }
        }
        return minimum;
    }

    static bool all_passing(const std::vector<ad_time>& ad_times, const std::size_t from, const std::size_t to)
    {
        for (auto i = from + 1; i + 1 < to + 1; ++i)
        {
            if (ad_times[i].arrival || ad_times[i].departure)
            {
                return false;
            }
        }
        return true;
    }

    // variables
};


timetable_vocabulary::timetable_vocabulary(std::unique_ptr<std::istream>&& p_input_stream) :
m_p_impl{ std::make_unique<impl>(std::move(p_input_stream)) }
{}

timetable_vocabulary::~timetable_vocabulary() = default;
