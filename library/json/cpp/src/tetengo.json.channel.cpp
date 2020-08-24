/*! \file
    \brief A channel.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/channel.hpp>


namespace tetengo::json
{
    class channel::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl() {}
    };


    channel::channel() : m_p_impl{ std::make_unique<impl>() } {}

    channel::~channel() = default;


}
