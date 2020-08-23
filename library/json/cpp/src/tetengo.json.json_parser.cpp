/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/element.hpp>
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


        // functions

        bool has_next() const
        {
            return false;
        }

        const element& peek() const
        {
            static const element element_{ { element::type_name_type::string, element::type_category_type::primitive },
                                           "",
                                           std::unordered_map<std::string, std::string>{} };
            return element_;
        }

        void next() {}


    private:
        // variables

        const std::unique_ptr<reader> m_p_reader;
    };


    json_parser::json_parser(std::unique_ptr<reader>&& p_reader) :
    m_p_impl{ std::make_unique<impl>(std::move(p_reader)) }
    {}

    json_parser::~json_parser() = default;

    bool json_parser::has_next() const
    {
        return m_p_impl->has_next();
    }

    const element& json_parser::peek() const
    {
        return m_p_impl->peek();
    }

    void json_parser::next()
    {
        m_p_impl->next();
    }


}
