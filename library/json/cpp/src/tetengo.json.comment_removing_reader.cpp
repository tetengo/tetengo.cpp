/*! \file
    \brief A comment removing reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <iterator>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/comment_removing_reader.hpp>
#include <tetengo/json/reader.hpp>


namespace tetengo::json
{
    class comment_removing_reader::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl(std::unique_ptr<reader>&& p_base_reader, std::string single_line_begin) :
        m_p_base_reader{ std::move(p_base_reader) },
            m_single_line_begin{ std::move(single_line_begin) },
            m_queue{}
        {
            if (!m_p_base_reader)
            {
                throw std::invalid_argument{ "p_base_reader is nullptr." };
            }
            if (m_single_line_begin.empty())
            {
                throw std::invalid_argument{ "single_line_begin is empty." };
            }
        }


        // functions

        bool has_next_impl() const
        {
            ensure_buffer_filled();
            return !m_queue.empty();
        }

        char get_impl() const
        {
            return 0;
        }

        void next_impl() {}


    private:
        // variables

        const std::unique_ptr<reader> m_p_base_reader;

        const std::string m_single_line_begin;

        mutable std::queue<char> m_queue;


        // functions

        void ensure_buffer_filled() const
        {
            if (!m_queue.empty())
            {
                return;
            }
            while (m_p_base_reader->has_next() && m_queue.empty())
            {
                auto line = read_line();
                line = remove_comment(std::move(line));
                std::for_each(std::begin(line), std::end(line), [this](const auto c) { m_queue.push(c); });
            }
        }

        std::string read_line() const
        {
            std::string line{};
            while (m_p_base_reader->has_next())
            {
                const auto char_ = m_p_base_reader->get();
                m_p_base_reader->next();
                line.push_back(char_);
                if (char_ == '\n')
                {
                    break;
                }
            }
            return line;
        }

        std::string remove_comment(std::string line) const
        {
            const auto  single_line_begin_offset = line.find(m_single_line_begin);
            const auto  line_feed_offset = line.find('\n');
            std::string comment_removed = line.substr(0, single_line_begin_offset);
            if (line_feed_offset < line.length())
            {
                comment_removed += line.substr(line_feed_offset);
            }
            return comment_removed;
        }
    };


    comment_removing_reader::comment_removing_reader(
        std::unique_ptr<reader>&& p_reader,
        std::string               single_line_begin) :
    m_p_impl{ std::make_unique<impl>(std::move(p_reader), std::move(single_line_begin)) }
    {}

    comment_removing_reader::~comment_removing_reader() = default;

    bool comment_removing_reader::has_next_impl() const
    {
        return m_p_impl->has_next_impl();
    }

    char comment_removing_reader::get_impl() const
    {
        return m_p_impl->get_impl();
    }

    void comment_removing_reader::next_impl()
    {
        m_p_impl->next_impl();
    }


}
