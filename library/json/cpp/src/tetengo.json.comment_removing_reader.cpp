/*! \file
    \brief A comment removing reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility> // IWYU pragma: keep

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
            if (std::empty(m_single_line_begin))
            {
                throw std::invalid_argument{ "single_line_begin is empty." };
            }
        }


        // functions

        bool has_next_impl() const
        {
            ensure_buffer_filled();
            return !std::empty(m_queue);
        }

        char peek_impl() const
        {
            ensure_buffer_filled();
            if (std::empty(m_queue))
            {
                throw std::logic_error{ "The current position is beyond the termination point." };
            }
            return m_queue.front();
        }

        void next_impl()
        {
            ensure_buffer_filled();
            if (std::empty(m_queue))
            {
                throw std::logic_error{ "The current position is beyond the termination point." };
            }
            m_queue.pop();
        }


    private:
        // static functions

        static std::size_t find_substring(const std::string_view& string_, const std::string_view& substring)
        {
            if (string_.length() < substring.length())
            {
                return std::string_view::npos;
            }

            auto quote = '\0';
            for (auto i = static_cast<std::size_t>(0); i < string_.length() - substring.length(); ++i)
            {
                if (quote == '\0' && (string_[i] == '\'' || string_[i] == '"'))
                {
                    quote = string_[i];
                    continue;
                }
                else if (quote != '\0')
                {
                    if (quote == string_[i])
                    {
                        quote = '\0';
                    }
                    continue;
                }

                auto found = true;
                for (auto j = static_cast<std::size_t>(0); j < substring.length(); ++j)
                {
                    if (string_[i + j] != substring[j])
                    {
                        found = false;
                        break;
                    }
                }

                if (found)
                {
                    return i;
                }
            }

            return std::string_view::npos;
        }


        // variables

        const std::unique_ptr<reader> m_p_base_reader;

        const std::string m_single_line_begin;

        mutable std::queue<char> m_queue;


        // functions

        void ensure_buffer_filled() const
        {
            if (!std::empty(m_queue))
            {
                return;
            }
            while (m_p_base_reader->has_next() && std::empty(m_queue))
            {
                auto line = read_line();
                line = remove_comment(line);
                std::for_each(std::begin(line), std::end(line), [this](const auto c) { m_queue.push(c); });
            }
        }

        std::string read_line() const
        {
            std::string line{};
            while (m_p_base_reader->has_next())
            {
                const auto char_ = m_p_base_reader->peek();
                m_p_base_reader->next();
                line.push_back(char_);
                if (char_ == '\n')
                {
                    break;
                }
            }
            return line;
        }

        std::string remove_comment(const std::string_view& line) const
        {
            const auto single_line_begin_offset = find_substring(line, m_single_line_begin);
            const auto line_feed_offset = line.find_first_of("\r\n");
            const auto comment_removed = line.substr(0, single_line_begin_offset);
            if (single_line_begin_offset != std::string_view::npos && line_feed_offset < line.length())
            {
                return std::string{ comment_removed } + std::string{ line.substr(line_feed_offset) };
            }
            else
            {
                return std::string{ comment_removed };
            }
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

    char comment_removing_reader::peek_impl() const
    {
        return m_p_impl->peek_impl();
    }

    void comment_removing_reader::next_impl()
    {
        m_p_impl->next_impl();
    }


}
