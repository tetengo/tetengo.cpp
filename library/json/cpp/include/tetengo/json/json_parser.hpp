/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_JSONPARSER_HPP)
#define TETENGO_JSON_JSONPARSER_HPP

#include <memory>

#include <boost/core/noncopyable.hpp>


namespace tetengo::json
{
    class reader;


    /*!
        \brief A JSON parser.
    */
    class json_parser : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a JSON parser.

            \param p_reader A unique pointer to a reader.

            \throw std::invalid_argument When p_reader is nullptr.
        */
        explicit json_parser(std::unique_ptr<reader>&& p_reader);

        /*!
            \brief Destroys the JSON parser.
        */
        ~json_parser();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
