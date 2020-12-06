/*! \file
    \brief An encoder.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_ENCODER_HPP)
#define TETENGO_TEXT_ENCODER_HPP

#include <boost/core/noncopyable.hpp>


namespace tetengo::text
{
    /*!
        \brief An encoder.
    */
    class encoder : private boost::noncopyable
    {
    public:
        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        [[nodiscard]] static const encoder& instance()
        {
            static const encoder singleton{};
            return singleton;
        }


        // functions
    };


}


#endif
