/*! \file
    \brief A channel.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <condition_variable>
#include <cstddef>
#include <exception>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/channel.hpp>
#include <tetengo/json/element.hpp>


namespace tetengo::json
{
    class channel::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(const std::size_t capacity) : m_capacity{ capacity }, m_queue{}, m_mutex{}, m_condition_variable{}
        {
            if (m_capacity == 0)
            {
                throw std::invalid_argument{ "capacity is 0." };
            }
        }


        // functions

        void insert(element element_)
        {
            std::unique_lock lock{ m_mutex };
            m_condition_variable.wait(lock, [this]() { return can_insert(); });

            if (can_take() && !m_queue.back())
            {
                m_condition_variable.notify_all();
                return;
            }

            m_queue.emplace(std::move(element_));

            m_condition_variable.notify_all();
        }

        void insert(std::exception_ptr&& p_exception)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            m_condition_variable.wait(lock, [this]() { return can_insert(); });

            if (can_take() && !m_queue.back())
            {
                m_condition_variable.notify_all();
                return;
            }

            m_queue.emplace(std::move(p_exception));

            m_condition_variable.notify_all();
        }

        const element& peek() const
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            m_condition_variable.wait(lock, [this]() { return can_take(); });
            if (closed_impl())
            {
                throw(std::logic_error{ "The channel is already closed." });
            }

            if (std::holds_alternative<element>(*m_queue.front()))
            {
                return std::get<element>(*m_queue.front());
            }
            else
            {
                assert(std::holds_alternative<std::exception_ptr>(*m_queue.front()));
                std::rethrow_exception(std::get<std::exception_ptr>(*m_queue.front()));
            }
        }

        void take()
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            m_condition_variable.wait(lock, [this]() { return can_take(); });
            if (closed_impl())
            {
                throw(std::logic_error{ "The channel is already closed." });
            }

            m_queue.pop();

            m_condition_variable.notify_all();
        }

        bool closed() const
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            m_condition_variable.wait(lock, [this]() { return can_take(); });
            return closed_impl();
        }

        void close()
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            m_condition_variable.wait(lock, [this]() { return can_insert(); });
            if (!can_take() || m_queue.back())
            {
                m_queue.push(std::nullopt);
            }
            m_condition_variable.notify_all();
        }


    private:
        // variables

        const std::size_t m_capacity;

        std::queue<std::optional<std::variant<element, std::exception_ptr>>> m_queue;

        mutable std::mutex m_mutex;

        mutable std::condition_variable m_condition_variable;


        // functions

        bool can_insert() const
        {
            return m_queue.size() < m_capacity;
        }

        bool can_take() const
        {
            return !m_queue.empty();
        }

        bool closed_impl() const
        {
            return !m_queue.empty() && !m_queue.front();
        }
    };


    channel::channel(const std::size_t capacity) : m_p_impl{ std::make_unique<impl>(capacity) } {}

    channel::~channel() = default;

    void channel::insert(element element_)
    {
        m_p_impl->insert(element_);
    }

    void channel::insert(std::exception_ptr&& p_exception)
    {
        m_p_impl->insert(std::move(p_exception));
    }

    const element& channel::peek() const
    {
        return m_p_impl->peek();
    }

    void channel::take()
    {
        m_p_impl->take();
    }

    bool channel::closed() const
    {
        return m_p_impl->closed();
    }

    void channel::close()
    {
        m_p_impl->close();
    }


}
