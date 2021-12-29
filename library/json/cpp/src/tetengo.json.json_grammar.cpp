/*! \file
    \brief A JSON grammar.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#if !defined(IWYU) // Boost.Spirit crashes IWYU 0.11

#include <cassert>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

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
        // types

        using primitive_type_type = json_grammar::primitive_type_type;

        using primitive_handler_type = json_grammar::primitive_handler_type;

        using structure_type_type = json_grammar::structure_type_type;

        using structure_open_close_type = json_grammar::structure_open_close_type;

        using structure_handler_type = json_grammar::structure_handler_type;


        // constructors and destructor

        impl(primitive_handler_type primitive_handler, structure_handler_type structure_handler) :
        m_primitive_handler{ std::move(primitive_handler) },
        m_structure_handler{ std::move(structure_handler) },
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

            const auto parsing_succeeded = boost::spirit::qi::parse(first, last, m_json_text);
            const auto all_consumed = first == last;
            return parsing_succeeded && all_consumed;
        }


    private:
        // types

        using iterator_type = boost::spirit::multi_pass<reader_iterator>;

        using rule_type = boost::spirit::qi::rule<iterator_type, std::string()>;

        struct call_primitieve_handler_type
        {
            const primitive_handler_type& m_handler;

            primitive_type_type m_type;

            call_primitieve_handler_type(const primitive_handler_type& handler, const primitive_type_type type) :
            m_handler{ handler },
            m_type{ type }
            {}

            void operator()(const std::string& attribute, const boost::spirit::qi::unused_type&, bool& pass) const
            {
                const std::string_view value =
                    m_type == primitive_type_type::string ? remove_quoration_marks(attribute) : attribute;
                pass = m_handler(m_type, value);
            }
        };

        struct call_structure_handler_type
        {
            const structure_handler_type& m_handler;

            structure_type_type m_type;

            structure_open_close_type m_open_close;

            call_structure_handler_type(
                const structure_handler_type&   handler,
                const structure_type_type       type,
                const structure_open_close_type open_close) :
            m_handler{ handler },
            m_type{ type },
            m_open_close{ open_close }
            {}

            void operator()(const std::string& attribute, const boost::spirit::qi::unused_type&, bool& pass) const
            {
                std::unordered_map<std::string_view, std::string_view> attributes{};
                if (m_type == structure_type_type::member && m_open_close == structure_open_close_type::open)
                {
                    attributes.insert(std::make_pair("name", remove_quoration_marks(attribute)));
                }
                pass = m_handler(m_type, m_open_close, attributes);
            }
        };


        // static functions

        static std::string_view remove_quoration_marks(const std::string_view& string_)
        {
            assert(string_.length() >= 2);
            assert(string_.front() == '"');
            assert(string_.back() == '"');
            return string_.substr(1, string_.length() - 2);
        }


        // variables

        primitive_handler_type m_primitive_handler;

        structure_handler_type m_structure_handler;

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
            m_value = m_false[call_primitieve_handler_type{ m_primitive_handler, primitive_type_type::boolean }] |
                      m_null[call_primitieve_handler_type{ m_primitive_handler, primitive_type_type::null }] |
                      m_true[call_primitieve_handler_type{ m_primitive_handler, primitive_type_type::boolean }] |
                      m_object | m_array |
                      m_number[call_primitieve_handler_type{ m_primitive_handler, primitive_type_type::number }] |
                      m_string[call_primitieve_handler_type{ m_primitive_handler, primitive_type_type::string }];
            m_false = qi::string("false");
            m_null = qi::string("null");
            m_true = qi::string("true");

            // 4. Objects
            m_object = m_begin_object[call_structure_handler_type{
                           m_structure_handler, structure_type_type::object, structure_open_close_type::open }] >>
                       -(m_member >> *(m_value_separator >> m_member)) >>
                       m_end_object[call_structure_handler_type{
                           m_structure_handler, structure_type_type::object, structure_open_close_type::close }];
            m_member = m_string[call_structure_handler_type{
                           m_structure_handler, structure_type_type::member, structure_open_close_type::open }] >>
                       m_name_separator >>
                       m_value[call_structure_handler_type{
                           m_structure_handler, structure_type_type::member, structure_open_close_type::close }];

            // 5. Arrays
            m_array = m_begin_array[call_structure_handler_type{
                          m_structure_handler, structure_type_type::array, structure_open_close_type::open }] >>
                      -(m_value >> *(m_value_separator >> m_value)) >>
                      m_end_array[call_structure_handler_type{
                          m_structure_handler, structure_type_type::array, structure_open_close_type::close }];

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


    json_grammar::json_grammar(primitive_handler_type primitive_handler, structure_handler_type structure_handler) :
    m_p_impl{ std::make_unique<impl>(std::move(primitive_handler), std::move(structure_handler)) }
    {}

    json_grammar::~json_grammar() = default;

    bool json_grammar::parse(reader& reader_) const
    {
        return m_p_impl->parse(reader_);
    }


}

#endif
