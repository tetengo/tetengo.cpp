/*! \file
    \brief A JSON grammar.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_JSONGRAMMAR_HPP)
#define TETENGO_JSON_JSONGRAMMAR_HPP

#include <functional>
#include <memory>
#include <string_view>

#include <boost/core/noncopyable.hpp>


namespace tetengo::json
{
    class reader;


    /*!
        \brief A JSON grammar.

        Based on <a href="https://tools.ietf.org/html/rfc8259">RFC 8259</a>.
    */
    class json_grammar : private boost::noncopyable
    {
    public:
        // types

        //! The primitive type type.
        enum class primitive_type_type
        {
            string, //!< A string.
            number, //!< A number.
            boolean, //!< A boolean.
            null, //!< A null.
        };

        //! The primitive handler type.
        using primitive_handler_type = std::function<bool(primitive_type_type, const std::string_view&)>;


        // constructors and destructor

        /*!
            \brief Creates a JSON grammar.

            \param primitive_handler A primitive element handler.
        */
        explicit json_grammar(primitive_handler_type primitive_handler);

        /*!
            \brief Destroys the JSON grammar.
        */
        ~json_grammar();


        // functions

        /*!
            \brief Parses a text.

            \param reader_ A reader.

            \retval true  When parsing is successful.
            \retval false Otherwise.
        */
        bool parse(reader& reader_) const;


    private:
        // types

        class impl;


        // variables

        std::unique_ptr<impl> m_p_impl;
    };


}


#endif
