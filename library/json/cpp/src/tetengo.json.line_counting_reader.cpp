/*! \file
    \brief A line counting reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <stdexcept>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/line_counting_reader.hpp>
#include <tetengo/json/reader.hpp>


namespace tetengo::json
{
    class line_counting_reader::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::unique_ptr<reader>&& p_base_reader) : m_p_base_reader{ std::move(p_base_reader) }
        {
            if (!m_p_base_reader)
            {
                throw std::invalid_argument{ "p_base_reader is nullptr." };
            }
        }


        // functions

        bool has_next_impl() const
        {
            throw std::logic_error{ "Implement it." };
        }

        char peek_impl() const
        {
            throw std::logic_error{ "Implement it." };
        }

        void next_impl()
        {
            throw std::logic_error{ "Implement it." };
        }


    private:
        // variables

        const std::unique_ptr<reader> m_p_base_reader;
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
