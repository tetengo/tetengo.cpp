/*! \file
    \brief A JSON grammar.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <boost/core/noncopyable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>

#include <tetengo/json/json_grammar.hpp>
#include <tetengo/json/reader_iterator.hpp>


namespace tetengo::json
{
    class reader;


    class json_grammar::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl() : m_json_text{}, m_ws{}
        {
            define_rules();
        }


        // functions

        bool parse(reader& /*reader_*/) const
        {
            return false;
        }


    private:
        // types

        using iterator_type = boost::spirit::multi_pass<reader_iterator>;

        using rule_type = boost::spirit::qi::rule<iterator_type>;


        // variables

        rule_type m_json_text;

        rule_type m_ws;


        // functions

        void define_rules()
        {
            namespace qi = boost::spirit::qi;

            m_ws = qi::char_(' ');
            m_json_text = m_ws;
        }
    };


    json_grammar::json_grammar() : m_p_impl{ std::make_unique<impl>() } {}

    json_grammar::~json_grammar() = default;

    bool json_grammar::parse(reader& reader_) const
    {
        return m_p_impl->parse(reader_);
    }


}
