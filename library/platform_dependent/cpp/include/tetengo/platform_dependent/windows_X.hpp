/*! \file
    \brief A Windows registry access.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PLATFORMDEPENDENT_WINDOWSREGISTRY_HPP)
#define TETENGO_PLATFORMDEPENDENT_WINDOWSREGISTRY_HPP

#include <memory>

#include <boost/core/noncopyable.hpp>


namespace tetengo::platform_dependent
{
    /*!
        \brief A Windows registry access.
    */
    class windows_registry : private boost::noncopyable
    {
    public:
        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        [[nodiscard]] static const windows_registry& instance();


        // constructors and destructor

        /*!
            \brief Destroys the Windows registry access.
        */
        ~windows_registry();


        // functions


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        windows_registry();
    };


}


#endif
