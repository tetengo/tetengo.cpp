/*! \file
    \brief A JSON grammar.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_JSONGRAMMAR_HPP)
#define TETENGO_JSON_JSONGRAMMAR_HPP

#include <memory>

#include <boost/core/noncopyable.hpp>


namespace tetengo::json
{
    class reader;


    /*!
        \brief A JSON grammar.
    */
    class json_grammar : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a JSON grammar.
        */
        json_grammar();

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
