/*! \file
    \brief A channel.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_CHANNEL_HPP)
#define TETENGO_JSON_CHANNEL_HPP

#include <cstddef>
#include <exception>
#include <memory>

#include <boost/core/noncopyable.hpp>


namespace tetengo::json
{
    class element;


    /*!
        \brief A channel.
    */
    class channel : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a channel.

            \param capacity A capacity. Must be greater than 0.

            \throw std::invalid_argument When capacity is 0.
        */
        explicit channel(std::size_t capacity);

        /*!
            \brief Destroys the JSON parser.
        */
        ~channel();


        // functions

        /*!
            \brief Inserts an element.

            The element inserted after a close() call are just discarded.

            \param element_ An element.
        */
        void insert(element element_);

        /*!
            \brief Inserts an exception.

            The exception inserted after a close() call are just discarded.

            \param p_exception An exception pointer.
        */
        void insert(std::exception_ptr&& p_exception);

        /*!
            \brief Returns the front element.

            \return The front element.

            \throw unspecified      An exception inserted with insert(std::exception_ptr&&).
            \throw std::logic_error When the channel is already closed.
        */
        [[nodiscard]] const element& peek() const;

        /*!
            \brief Takes an element.

            \throw std::logic_error When the channel is already closed.
        */
        void take();

        /*!
            \brief Returns true the channel is closed.

            \retval true  When the channel is closed.
            \retval false Otherwise.
        */
        [[nodiscard]] bool closed() const;

        /*!
            \brief Closes the channel.
        */
        void close();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
