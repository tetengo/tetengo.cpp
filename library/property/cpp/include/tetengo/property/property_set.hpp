/*! \file
    \brief A property set.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_PROPERTYSET_HPP)
#define TETENGO_PROPERTY_PROPERTYSET_HPP

#include <memory>

#include <boost/core/noncopyable.hpp>


namespace tetengo::property
{
    class storage_loader;


    /*!
        \brief A property set.
    */
    class property_set : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a property set.

            \param p_storage_loader A unique pointer to a storage loader.
        */
        explicit property_set(std::unique_ptr<storage_loader>&& p_storage_loader);

        /*!
            \brief Destroys the property set.
        */
        virtual ~property_set();


        // functions


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions
    };


}


#endif
