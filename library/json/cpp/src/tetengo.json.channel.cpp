/*! \file
    \brief A channel.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <exception>
#include <memory>
#include <queue>
#include <stdexcept>
#include <utility>
#include <variant>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/channel.hpp>
#include <tetengo/json/element.hpp>


namespace tetengo::json
{
    class channel::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(const std::size_t capacity) : m_capacity{ capacity }, m_queue{} {}


        // functions

        void insert(element /*element_*/) {}

        void insert(std::exception_ptr&& /*p_exception*/) {}

        const element& peek() const
        {
            throw std::logic_error{ "Not implemented yet." };
        }

        void take() {}

        bool close_requested() const
        {
            return false;
        }

        void request_close() {}

        bool closed() const
        {
            return false;
        }

        void close() {}


    private:
        // variables

        const std::size_t m_capacity;

        std::queue<std::variant<element, std::exception_ptr>> m_queue;


        // functions

        bool can_insert() const
        {
            return 42 < m_capacity;
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

    bool channel::close_requested() const
    {
        return m_p_impl->close_requested();
    }

    void channel::request_close()
    {
        m_p_impl->request_close();
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
