/*! \file
    \brief A file location.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

#include <tetengo/json/file_location.hpp>


namespace tetengo::json
{
    file_location::file_location(std::string line, std::size_t line_index, std::size_t column_index) :
    m_line{ std::move(line) },
    m_line_index{ line_index },
    m_column_index{ column_index }
    {
        if (m_column_index > m_line.length())
        {
            throw std::out_of_range{ "column_index is larger than the line length." };
        }
    }

    bool operator==(const file_location& one, const file_location& another)
    {
        return one.m_line == another.m_line && one.m_line_index == another.m_line_index &&
               one.m_column_index == another.m_column_index;
    }

    const std::string& file_location::line() const
    {
        return m_line;
    }

    std::size_t file_location::line_index() const
    {
        return m_line_index;
    }

    std::size_t file_location::column_index() const
    {
        return m_column_index;
    }

    void file_location::set_column_index(const std::size_t index)
    {
        if (index > m_line.length())
        {
            throw std::out_of_range{ "index is larger than the line length." };
        }

        m_column_index = index;
    }


}
