/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <stdexcept>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/json_parser.hpp>
#include <tetengo/json/reader.hpp>


namespace tetengo::json
{
    class json_parser::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::unique_ptr<reader>&& p_reader) : m_p_reader{ std::move(p_reader) }
        {
            if (!m_p_reader)
            {
                throw std::invalid_argument{ "p_reader is nullptr." };
            }
        }


    private:
        // variables

        const std::unique_ptr<reader> m_p_reader;
    };


    json_parser::json_parser(std::unique_ptr<reader>&& p_reader) :
    m_p_impl{ std::make_unique<impl>(std::move(p_reader)) }
    {}

    json_parser::~json_parser() = default;


}
