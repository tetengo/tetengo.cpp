/*! \file
    \brief A line counting reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/line_counting_reader.hpp>
#include <tetengo/json/reader.hpp>


namespace tetengo::json
{
    location::location(std::string_view line, const std::size_t line_index, const std::size_t column_index) :
    m_line{ std::move(line) },
        m_line_index{ line_index },
        m_column_index{ column_index }
    {}

    const std::string_view& location::line() const
    {
        return m_line;
    }

    std::size_t location::line_index() const
    {
        return m_line_index;
    }

    std::size_t location::column_index() const
    {
        return m_column_index;
    }


    class line_counting_reader::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::unique_ptr<reader>&& p_base_reader) :
        m_p_base_reader{ std::move(p_base_reader) },
            m_line{},
            m_current_position{ std::end(m_line) }
        {
            if (!m_p_base_reader)
            {
                throw std::invalid_argument{ "p_base_reader is nullptr." };
            }
        }


        // functions

        bool has_next_impl() const
        {
            ensure_line_loaded();
            return m_current_position != std::end(m_line);
        }

        char peek_impl() const
        {
            ensure_line_loaded();
            if (m_current_position == std::end(m_line))
            {
                throw std::logic_error{ "The current position is beyond the termination point." };
            }
            return *m_current_position;
        }

        void next_impl()
        {
            ensure_line_loaded();
            if (m_current_position == std::end(m_line))
            {
                throw std::logic_error{ "The current position is beyond the termination point." };
            }
            ++m_current_position;
        }


    private:
        // variables

        const std::unique_ptr<reader> m_p_base_reader;

        mutable std::vector<char> m_line;

        mutable std::vector<char>::const_iterator m_current_position;


        // functions

        void ensure_line_loaded() const
        {
            if (m_current_position != std::end(m_line))
            {
                return;
            }

            m_line.clear();
            while (m_p_base_reader->has_next() && (std::empty(m_line) || m_line.back() != '\n'))
            {
                m_line.push_back(m_p_base_reader->peek());
                m_p_base_reader->next();
            }

            m_current_position = std::begin(m_line);
        }
    };


    line_counting_reader::line_counting_reader(std::unique_ptr<reader>&& p_reader) :
    m_p_impl{ std::make_unique<impl>(std::move(p_reader)) }
    {}

    line_counting_reader::~line_counting_reader() = default;

    bool line_counting_reader::has_next_impl() const
    {
        return m_p_impl->has_next_impl();
    }

    char line_counting_reader::peek_impl() const
    {
        return m_p_impl->peek_impl();
    }

    void line_counting_reader::next_impl()
    {
        m_p_impl->next_impl();
    }


}
