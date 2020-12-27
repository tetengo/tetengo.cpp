/*! \file
    \brief A JSON grammar.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_JSONGRAMMAR_HPP)
#define TETENGO_JSON_JSONGRAMMAR_HPP

#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>

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

        /*!
            \brief The primitive handler type.

            \param type  A type.
            \param value A value.

            \retval true  When the parsing is successful.
            \retval false Otherwise.
        */
        using primitive_handler_type = std::function<bool(primitive_type_type type, const std::string_view& value)>;

        //! The structure type type.
        enum class structure_type_type
        {
            object, //!< An object.
            member, //!< A member.
            array, //!< An array.
        };

        //! The structure open-close type.
        enum class structure_open_close_type
        {
            open, //!< Opening.
            close, //!< Closing.
        };

        /*!
            \brief The structure handler type.

            \param type       A type.
            \param open_close An open or close state.
            \param value      A value.

            \retval true  When the parsing is successful.
            \retval false Otherwise.
        */
        using structure_handler_type = std::function<bool(
            structure_type_type                                           type,
            structure_open_close_type                                     open_close,
            const std::unordered_map<std::string_view, std::string_view>& attributes)>;


        // constructors and destructor

        /*!
            \brief Creates a JSON grammar.

            \param primitive_handler A primitive element handler.
            \param structure_handler A structure element handler.
        */
        json_grammar(primitive_handler_type primitive_handler, structure_handler_type structure_handler);

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
        [[nodiscard]] bool parse(reader& reader_) const;


    private:
        // types

        class impl;


        // variables

        std::unique_ptr<impl> m_p_impl;
    };


}


#endif
