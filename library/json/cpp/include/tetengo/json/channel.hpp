/*! \file
    \brief A channel.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_CHANNEL_HPP)
#define TETENGO_JSON_CHANNEL_HPP

#include <cstddef>
#include <memory>

#include <boost/core/noncopyable.hpp>


namespace tetengo::json
{
    /*!
        \brief A channel.
    */
    class channel : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a channel.

            \param capacity A capacity.
        */
        explicit channel(std::size_t capacity);

        /*!
            \brief Destroys the JSON parser.
        */
        ~channel();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
