/*! \file
    \brief A strea, reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/stream_reader.hpp>


namespace tetengo::json
{
    class stream_reader::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl() {}


        // functions

        bool has_next_impl() const
        {
            return false;
        }

        char get_impl() const
        {
            return 0;
        }

        void next_impl() {}


    private:
        // variables
    };


    stream_reader::stream_reader() : m_p_impl{ std::make_unique<impl>() } {}

    stream_reader::~stream_reader() = default;

    bool stream_reader::has_next_impl() const
    {
        return m_p_impl->has_next_impl();
    }

    char stream_reader::get_impl() const
    {
        return m_p_impl->get_impl();
    }

    void stream_reader::next_impl()
    {
        m_p_impl->next_impl();
    }


}
