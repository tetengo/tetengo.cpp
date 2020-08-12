/*! \file
    \brief A reader iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <stdexcept>

#include <tetengo/json/reader.hpp>
#include <tetengo/json/reader_iterator.hpp>


namespace tetengo::json
{
    reader_iterator::reader_iterator() : m_p_reader{ nullptr }, m_increment_count{ 0 } {}

    reader_iterator::reader_iterator(reader& reader_) : m_p_reader{ &reader_ }, m_increment_count{ 0 } {}

    char reader_iterator::dereference() const
    {
        if (!m_p_reader || !m_p_reader->has_next())
        {
            throw std::logic_error{ "No more character." };
        }
        return m_p_reader->peek();
    }

    bool reader_iterator::equal(const reader_iterator& another) const
    {
        if (m_p_reader && another.m_p_reader)
        {
            if (m_p_reader == another.m_p_reader)
            {
                return m_p_reader->has_next() == another.m_p_reader->has_next() &&
                       m_increment_count == another.m_increment_count;
            }
            else
            {
                return false;
            }
        }
        else if (m_p_reader)
        {
            return !m_p_reader->has_next();
        }
        else if (another.m_p_reader)
        {
            return !another.m_p_reader->has_next();
        }
        else
        {
            return true;
        }
    }

    void reader_iterator::increment()
    {
        if (!m_p_reader || !m_p_reader->has_next())
        {
            throw std::logic_error{ "No more character." };
        }
        m_p_reader->next();
        ++m_increment_count;
    }


}
