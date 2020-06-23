/*! \file
    \brief A timetable vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <algorithm>
#include <cassert>
#include <istream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/core/noncopyable.hpp>

#include "timetable_vocabulary.hpp"


namespace
{
    struct station
    {
        std::string name;

        std::string telegram_code;

        station(std::string&& name, std::string&& telegram_code) : name{ name }, telegram_code{ telegram_code } {}
    };


}


class timetable_vocabulary::impl : private boost::noncopyable
{
public:
    // constructors and destructor

    explicit impl(std::unique_ptr<std::istream>&& p_input_stream)
    {
        assert(p_input_stream);
        parse_input(*p_input_stream);
    }


private:
    // static functions

    static void parse_input(std::istream& input_stream)
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
        line1.erase(std::begin(line1));
        line2.erase(std::begin(line2));
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


    // variables
};


timetable_vocabulary::timetable_vocabulary(std::unique_ptr<std::istream>&& p_input_stream) :
m_p_impl{ std::make_unique<impl>(std::move(p_input_stream)) }
{}

timetable_vocabulary::~timetable_vocabulary() = default;
