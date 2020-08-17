/*! \file
    \brief A JSON grammar.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(IWYU) // Boost.Spirit crashes IWYU 0.11

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

        impl() :
        m_json_text{},
            m_begin_array{},
            m_begin_object{},
            m_end_array{},
            m_end_object{},
            m_name_separator{},
            m_value_separator{},
            m_ws{},
            m_value{},
            m_false{},
            m_null{},
            m_true{},
            m_object{},
            m_member{},
            m_array{},
            m_number{},
            m_decimal_point{},
            m_digit1_9{},
            m_e{},
            m_exp{},
            m_frac{},
            m_int{},
            m_minus{},
            m_plus{},
            m_zero{},
            m_string{},
            m_char{},
            m_escape{},
            m_quotation_mark{},
            m_unescaped{}
        {
            define_rules();
        }


        // functions

        bool parse(reader& reader_) const
        {
            auto       first = boost::spirit::make_default_multi_pass(reader_iterator{ reader_ });
            const auto last = boost::spirit::make_default_multi_pass(reader_iterator{});

            return boost::spirit::qi::parse(first, last, m_json_text);
        }


    private:
        // types

        using iterator_type = boost::spirit::multi_pass<reader_iterator>;

        using rule_type = boost::spirit::qi::rule<iterator_type>;


        // variables

        rule_type m_json_text;

        rule_type m_begin_array;

        rule_type m_begin_object;

        rule_type m_end_array;

        rule_type m_end_object;

        rule_type m_name_separator;

        rule_type m_value_separator;

        rule_type m_ws;

        rule_type m_value;

        rule_type m_false;

        rule_type m_null;

        rule_type m_true;

        rule_type m_object;

        rule_type m_member;

        rule_type m_array;

        rule_type m_number;

        rule_type m_decimal_point;

        rule_type m_digit1_9;

        rule_type m_e;

        rule_type m_exp;

        rule_type m_frac;

        rule_type m_int;

        rule_type m_minus;

        rule_type m_plus;

        rule_type m_zero;

        rule_type m_string;

        rule_type m_char;

        rule_type m_escape;

        rule_type m_quotation_mark;

        rule_type m_unescaped;


        // functions

        void define_rules()
        {
            namespace qi = boost::spirit::qi;

            // 2. Grammar
            m_json_text = m_ws >> m_value >> m_ws;
            m_begin_array = m_ws >> qi::char_('[') >> m_ws;
            m_begin_object = m_ws >> qi::char_('{') >> m_ws;
            m_end_array = m_ws >> qi::char_(']') >> m_ws;
            m_end_object = m_ws >> qi::char_('}') >> m_ws;
            m_name_separator = m_ws >> qi::char_(':') >> m_ws;
            m_value_separator = m_ws >> qi::char_(',') >> m_ws;
            m_ws = *(qi::char_(' ') | qi::char_('\t') | qi::char_('\n') | qi::char_('\r'));

            // 3. Values
            m_value = m_false | m_null | m_true | m_object | m_array | m_number | m_string;
            m_false = qi::string("false");
            m_null = qi::string("null");
            m_true = qi::string("true");

            // 4. Objects
            m_object = m_begin_object >> -(m_member >> *(m_value_separator >> m_member)) >> m_end_object;
            m_member = m_string >> m_name_separator >> m_value;

            // 5. Arrays
            m_array = m_begin_array >> -(m_value >> *(m_value_separator >> m_value)) >> m_end_array;

            // 6. Numbers
            m_number = -m_minus >> m_int >> -m_frac >> -m_exp;
            m_decimal_point = qi::char_('.');
            m_digit1_9 = qi::char_('1', '9');
            m_e = qi::char_('e') | qi::char_('E');
            m_exp = m_e >> -(m_minus | m_plus) >> +qi::digit;
            m_frac = m_decimal_point >> +qi::digit;
            m_int = m_zero | (m_digit1_9 >> *qi::digit);
            m_minus = qi::char_('-');
            m_plus = qi::char_('+');
            m_zero = qi::char_('0');

            // 7. Strings
            m_string = m_quotation_mark >> *m_char >> m_quotation_mark;
            m_char = m_unescaped | (m_escape >> (qi::char_('"') | qi::char_('\\') | qi::char_('/') | qi::char_('b') |
                                                 qi::char_('f') | qi::char_('n') | qi::char_('r') | qi::char_('t') |
                                                 (qi::char_('u') >> qi::repeat(4)[qi::xdigit])));
            m_escape = qi::char_('\\');
            m_quotation_mark = qi::char_('"');
            m_unescaped =
                qi::char_ - qi::char_(static_cast<char>(0x00), static_cast<char>(0x19)) - m_quotation_mark - m_escape;
        }
    };


    json_grammar::json_grammar() : m_p_impl{ std::make_unique<impl>() } {}

    json_grammar::~json_grammar() = default;

    bool json_grammar::parse(reader& reader_) const
    {
        return m_p_impl->parse(reader_);
    }


}

#endif
