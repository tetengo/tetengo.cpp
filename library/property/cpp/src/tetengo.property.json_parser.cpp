/*! \file
    \brief A json parser.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <boost/core/noncopyable.hpp>

#include "tetengo.property.json_parser.hpp"


namespace tetengo::property
{
    class json_parser::impl : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = json_parser::value_map_type;


        // static functions

        static const json_parser& instance()
        {
            static const json_parser singleton{};
            return singleton;
        }


        // functions

        value_map_type parse(tetengo::json::json_parser& /*parser*/) const
        {
            value_map_type value_map{};

            return value_map;
        }
    };


    const json_parser& json_parser::instance()
    {
        return impl::instance();
    }

    json_parser::~json_parser() = default;

    json_parser::value_map_type json_parser::parse(tetengo::json::json_parser& parser) const
    {
        return m_p_impl->parse(parser);
    }

    json_parser::json_parser() : m_p_impl{ std::make_unique<impl>() } {}


}
