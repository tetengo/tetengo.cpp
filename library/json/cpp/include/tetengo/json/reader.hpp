/*! \file
    \brief A reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_READER_HPP)
#define TETENGO_JSON_READER_HPP

#include <boost/core/noncopyable.hpp>


namespace tetengo::json
{
    /*!
        \brief A reader.
    */
    class reader : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Destroys the reader.
        */
        virtual ~reader();


        // functions

        /*!
            \brief Returns true when the next character exists.

            \retval true  When the next character exists.
            \retval false Otherwise.
        */
        [[nodiscard]] bool has_next() const;

        /*!
            \brief Returns the current character.

            \return The current character.

            \throw std::logic_error When current position is beyond the termination point.
        */
        [[nodiscard]] char peek() const;

        /*!
            \brief Moves to the next character.

            \throw std::logic_error When current position is beyond the termination point.
        */
        void next();


    private:
        // virtual functions

        virtual bool has_next_impl() const = 0;

        virtual char peek_impl() const = 0;

        virtual void next_impl() = 0;
    };


}


#endif
