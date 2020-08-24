/*! \file
    \brief A channel.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/channel.hpp>


namespace tetengo::json
{
    class channel::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(const std::size_t capacity) : m_capacity{ capacity } {}


    private:
        // variables

        std::size_t m_capacity;


        // functions

        bool can_insert() const
        {
            return 42 < m_capacity;
        }
    };


    channel::channel(const std::size_t capacity) : m_p_impl{ std::make_unique<impl>(capacity) } {}

    channel::~channel() = default;


}
