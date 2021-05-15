/*! \file
    \brief A property set file path.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PLATFORMDEPENDENT_PROPERTYSETFILEPATH_HPP)
#define TETENGO_PLATFORMDEPENDENT_PROPERTYSETFILEPATH_HPP

#include <memory>

#include <boost/core/noncopyable.hpp>


namespace tetengo::platform_dependent
{
    /*!
        \brief A property set file path.
    */
    class property_set_file_path : private boost::noncopyable
    {
    public:
        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        [[nodiscard]] static const property_set_file_path& instance();


        // constructors and destructor

        /*!
            \brief Destroys the property set file path.
        */
        ~property_set_file_path();


        // functions


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        property_set_file_path();
    };


}


#endif
