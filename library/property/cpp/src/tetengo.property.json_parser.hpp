/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_JSONPARSER_HPP_)
#define TETENGO_PROPERTY_JSONPARSER_HPP_

#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/storage.hpp>

namespace tetengo::json
{
    class json_parser;
}


namespace tetengo::property
{
    class json_parser : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = storage::value_map_type;


        // static functions

        static const json_parser& instance();


        // constructors and destructor

        ~json_parser();


        // functions

        value_map_type parse(tetengo::json::json_parser& parser) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        json_parser();
    };


}


#endif
