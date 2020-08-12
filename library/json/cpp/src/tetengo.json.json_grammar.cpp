/*! \file
    \brief A JSON grammar.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/json/json_grammar.hpp>


namespace tetengo::json
{
    class reader;


    class json_grammar::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl() {}


        // functions

        bool parse(reader& /*reader_*/) const
        {
            return false;
        }


    private:
    };


    json_grammar::json_grammar() : m_p_impl{ std::make_unique<impl>() } {}

    json_grammar::~json_grammar() = default;

    bool json_grammar::parse(reader& reader_) const
    {
        return m_p_impl->parse(reader_);
    }


}
