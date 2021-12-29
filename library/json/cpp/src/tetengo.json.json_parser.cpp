/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <cstddef>
#include <exception>
#include <functional>
#include <iterator>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/channel.hpp>
#include <tetengo/json/comment_removing_reader.hpp>
#include <tetengo/json/element.hpp>
#include <tetengo/json/json_grammar.hpp>
#include <tetengo/json/json_parser.hpp>
#include <tetengo/json/line_counting_reader.hpp>
#include <tetengo/json/reader.hpp>


namespace tetengo::json
{
    class json_parser::impl : private boost::noncopyable
    {
    public:
        // static functions

        static constexpr std::size_t default_buffer_capacity()
        {
            return 100;
        }


        // constructors and destructor

        impl(std::unique_ptr<reader>&& p_reader, const std::size_t buffer_capacity) :
        m_p_reader{ build_decorated_reader(std::move(p_reader)) },
        m_p_worker{},
        m_channel{ buffer_capacity },
        m_parsing_abortion_requested{ false },
        m_mutex{}
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
                request_parsing_abortion();
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
            return !m_channel.closed();
        }

        const element& peek() const
        {
            if (!has_next())
            {
                throw std::logic_error{ "No more element." };
            }
            return m_channel.peek();
        }

        void next()
        {
            if (!has_next())
            {
                throw std::logic_error{ "No more element." };
            }
            m_channel.take();
        }


    private:
        // static functions

        static std::unique_ptr<reader> build_decorated_reader(std::unique_ptr<reader>&& p_base_reader)
        {
            auto p_line_counting = std::make_unique<line_counting_reader>(std::move(p_base_reader));
            auto p_comment_removing = std::make_unique<comment_removing_reader>(std::move(p_line_counting), "#");
            return p_comment_removing;
        }

        static const line_counting_reader& as_line_counting(const reader& reader_)
        {
            assert(dynamic_cast<const line_counting_reader*>(&reader_.base_reader()));
            return dynamic_cast<const line_counting_reader&>(reader_.base_reader());
        }

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
            switch (const element::type_category_type element_category =
                        structure_open_close == json_grammar::structure_open_close_type::open ?
                            element::type_category_type::structure_open :
                            element::type_category_type::structure_close;
                    structure_type)
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
            element_attributes.reserve(std::size(structure_attributes));
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

        bool m_parsing_abortion_requested;

        mutable std::mutex m_mutex;


        // functions

        void worker_procedure()
        {
            try
            {
                const json_grammar grammar_{
                    std::bind(&impl::on_primitive, this, std::placeholders::_1, std::placeholders::_2),
                    std::bind(
                        &impl::on_structure, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
                };

                const auto successful = grammar_.parse(*m_p_reader);
                if (!successful)
                {
                    m_channel.insert(std::make_exception_ptr(std::runtime_error{ "JSON syntax error." }));
                }
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
            if (parsing_abortion_requested())
            {
                return false;
            }

            element element_{ to_element_type(type),
                              std::string{ value },
                              std::unordered_map<std::string, std::string>{},
                              as_line_counting(*m_p_reader).get_location() };
            m_channel.insert(std::move(element_));
            return true;
        }

        bool on_structure(
            const json_grammar::structure_type_type                       type,
            const json_grammar::structure_open_close_type                 open_close,
            const std::unordered_map<std::string_view, std::string_view>& attributes)
        {
            if (parsing_abortion_requested())
            {
                return false;
            }

            element element_{ to_element_type(type, open_close),
                              std::string{},
                              to_element_attributes(attributes),
                              as_line_counting(*m_p_reader).get_location() };
            m_channel.insert(std::move(element_));
            return true;
        }

        bool parsing_abortion_requested() const
        {
            const std::unique_lock<std::mutex> lock{ m_mutex };
            return m_parsing_abortion_requested;
        }

        void request_parsing_abortion()
        {
            const std::unique_lock<std::mutex> lock{ m_mutex };
            m_parsing_abortion_requested = true;
        }
    };


    std::size_t json_parser::default_buffer_capacity()
    {
        return impl::default_buffer_capacity();
    }

    json_parser::json_parser(
        std::unique_ptr<reader>&& p_reader,
        const std::size_t         buffer_capacity /*= default_buffer_capacity()*/) :
    m_p_impl{ std::make_unique<impl>(std::move(p_reader), buffer_capacity) }
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
