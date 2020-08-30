/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/channel.hpp>
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

        explicit impl(std::unique_ptr<reader>&& p_reader) :
        m_p_reader{ std::move(p_reader) },
            m_p_worker{},
            m_channel{ 10 }
        {
            if (!m_p_reader)
            {
                throw std::invalid_argument{ "p_reader is nullptr." };
            }
            m_p_worker = std::make_unique<std::thread>(&impl::worker_procedure, this);
        }

        ~impl()
        {
            try
            {
                while (!m_channel.closed())
                {
                    m_channel.take();
                }
                if (m_p_worker && m_p_worker->joinable())
                {
                    m_p_worker->join();
                }
            }
            catch (...)
            {}
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
        // static functions

        static element::type_type to_element_type(const json_grammar::primitive_type_type primitive_type)
        {
            switch (primitive_type)
            {
            case json_grammar::primitive_type_type::string:
                return { element::type_name_type::string, element::type_category_type::primitive };
            case json_grammar::primitive_type_type::number:
                return { element::type_name_type::number, element::type_category_type::primitive };
            case json_grammar::primitive_type_type::boolean:
                return { element::type_name_type::boolean, element::type_category_type::primitive };
            default:
                assert(primitive_type == json_grammar::primitive_type_type::null);
                return { element::type_name_type::null, element::type_category_type::primitive };
            }
        }

        static element::type_type to_element_type(
            const json_grammar::structure_type_type       structure_type,
            const json_grammar::structure_open_close_type structure_open_close)
        {
            const element::type_category_type element_category =
                structure_open_close == json_grammar::structure_open_close_type::open ?
                    element::type_category_type::structure_open :
                    element::type_category_type::structure_close;
            switch (structure_type)
            {
            case json_grammar::structure_type_type::object:
                return { element::type_name_type::object, element_category };
            case json_grammar::structure_type_type::member:
                return { element::type_name_type::member, element_category };
            default:
                assert(structure_type == json_grammar::structure_type_type::array);
                return { element::type_name_type::array, element_category };
            }
        }

        static std::unordered_map<std::string, std::string>
        to_element_attributes(const std::unordered_map<std::string_view, std::string_view>& structure_attributes)
        {
            std::unordered_map<std::string, std::string> element_attributes{};
            element_attributes.reserve(structure_attributes.size());
            for (const auto& i: structure_attributes)
            {
                element_attributes.insert(i);
            }
            return element_attributes;
        }


        // variables

        const std::unique_ptr<reader> m_p_reader;

        std::unique_ptr<std::thread> m_p_worker;

        channel m_channel;


        // functions

        void worker_procedure()
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
                m_channel.close();
            }
            catch (...)
            {
                m_channel.insert(std::current_exception());
                m_channel.close();
            }
        }

        bool on_primitive(const json_grammar::primitive_type_type type, const std::string_view& value)
        {
            element element_{ to_element_type(type),
                              std::string{ value },
                              std::unordered_map<std::string, std::string>{} };
            m_channel.insert(std::move(element_));
            return true;
        }

        bool on_structure(
            const json_grammar::structure_type_type                       type,
            const json_grammar::structure_open_close_type                 open_close,
            const std::unordered_map<std::string_view, std::string_view>& attributes)
        {
            element element_{ to_element_type(type, open_close), std::string{}, to_element_attributes(attributes) };
            m_channel.insert(std::move(element_));
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
