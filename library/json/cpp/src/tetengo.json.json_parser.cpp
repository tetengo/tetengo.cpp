/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/element.hpp>
#include <tetengo/json/json_grammar.hpp>
#include <tetengo/json/json_parser.hpp>
#include <tetengo/json/reader.hpp>


namespace tetengo::json
{
    class json_parser::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::unique_ptr<reader>&& p_reader) : m_p_reader{ std::move(p_reader) }, m_p_worker{}
        {
            if (!m_p_reader)
            {
                throw std::invalid_argument{ "p_reader is nullptr." };
            }
            m_p_worker = std::make_unique<std::thread>(&impl::worker_precedure, this);
        }

        ~impl()
        {
            if (m_p_worker && m_p_worker->joinable())
            {
                m_p_worker->join();
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

        std::unique_ptr<std::thread> m_p_worker;


        // functions

        void worker_precedure()
        {
            try
            {
                const json_grammar grammar_{
                    [this](const json_grammar::primitive_type_type type, const std::string_view& value) {
                        return on_primitive(type, value);
                    },
                    [this](
                        const json_grammar::structure_type_type                       type,
                        const json_grammar::structure_open_close_type                 open_close,
                        const std::unordered_map<std::string_view, std::string_view>& attributes) {
                        return on_structure(type, open_close, attributes);
                    }
                };

                grammar_.parse(*m_p_reader);
            }
            catch (...)
            {}
        }

        bool on_primitive(const json_grammar::primitive_type_type /*type*/, const std::string_view& /*value*/)
        {
            return true;
        }

        bool on_structure(
            const json_grammar::structure_type_type /*type*/,
            const json_grammar::structure_open_close_type /*open_close*/,
            const std::unordered_map<std::string_view, std::string_view>& /*attributes*/)
        {
            return true;
        }
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
