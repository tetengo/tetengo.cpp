/*! \file
    \brief A line counting reader.

    $12026 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/file_location.hpp>
#include <tetengo/json/line_counting_reader.hpp>
#include <tetengo/json/reader.hpp>


namespace tetengo::json
{
    class line_counting_reader::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::unique_ptr<reader>&& p_base_reader) :
        m_p_base_reader{ std::move(p_base_reader) },
        m_line{},
        m_line_index{ 0 },
        m_column_index{ 0 },
        m_file_location{ std::string{ std::data(m_line), std::size(m_line) }, m_line_index, m_column_index }
        {
            if (!m_p_base_reader)
            {
                throw std::invalid_argument{ "p_base_reader is nullptr." };
            }
        }


        // functions

        const file_location& get_location() const
        {
            ensure_line_loaded();
            if (m_column_index == std::size(m_line) + 1)
            {
                throw std::logic_error{ "The current position is beyond the termination point." };
            }
            return m_file_location;
        }

        bool has_next_impl() const
        {
            ensure_line_loaded();
            return m_column_index < std::size(m_line);
        }

        char peek_impl() const
        {
            ensure_line_loaded();
            if (m_column_index == std::size(m_line))
            {
                throw std::logic_error{ "The current position is beyond the termination point." };
            }
            return m_line[m_column_index];
        }

        void next_impl()
        {
            ensure_line_loaded();
            if (m_column_index == std::size(m_line))
            {
                throw std::logic_error{ "The current position is beyond the termination point." };
            }
            ++m_column_index;
            m_file_location.set_column_index(m_column_index);
        }

        const reader& base_reader_impl() const
        {
            assert(m_p_base_reader);
            return *m_p_base_reader;
        }


    private:
        // variables

        const std::unique_ptr<reader> m_p_base_reader;

        mutable std::vector<char> m_line;

        mutable std::size_t m_line_index;

        mutable std::size_t m_column_index;

        mutable file_location m_file_location;


        // functions

        void ensure_line_loaded() const
        {
            if (m_column_index < std::size(m_line))
            {
                return;
            }
            if (!m_p_base_reader->has_next())
            {
                return;
            }

            m_line.clear();
            while (m_p_base_reader->has_next() && (std::empty(m_line) || m_line.back() != '\n'))
            {
                m_line.push_back(m_p_base_reader->peek());
                m_p_base_reader->next();
            }

            m_column_index = 0;
            ++m_line_index;

            m_file_location = file_location{ std::string{ std::data(m_line), std::size(m_line) }, m_line_index - 1, 0 };
        }
    };


    line_counting_reader::line_counting_reader(std::unique_ptr<reader>&& p_reader) :
    m_p_impl{ std::make_unique<impl>(std::move(p_reader)) }
    {}

    line_counting_reader::~line_counting_reader() = default;

    const file_location& line_counting_reader::get_location() const
    {
        return m_p_impl->get_location();
    }

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

    const reader& line_counting_reader::base_reader_impl() const
    {
        return m_p_impl->base_reader_impl();
    }


}
