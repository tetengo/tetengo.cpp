/*! \file
    \brief A comment removing reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <stdexcept>
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
            return false;
        }

        char get_impl() const
        {
            return 0;
        }

        void next_impl() {}


    private:
        // variables

        const std::unique_ptr<reader> m_p_base_reader;
    };


    comment_removing_reader::comment_removing_reader(std::unique_ptr<reader>&& p_reader) :
    m_p_impl{ std::make_unique<impl>(std::move(p_reader)) }
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
