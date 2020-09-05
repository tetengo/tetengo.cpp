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
    class element;
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


        // functions

        /*!
            \brief Returns true when the next element exists.

            \return true  When the next element exists.
            \return false Otherwise.
        */
        bool has_next() const;

        /*!
            \brief Returns the current element.

            \return The current element.

            \throw std::logic_error When the current position is beyond the termination point.
        */
        const element& peek() const;

        /*!
            \brief Moves to the next element.

            \throw std::logic_error When the current position is beyond the termination point.
        */
        void next();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
