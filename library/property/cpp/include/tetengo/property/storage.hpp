/*! \file
    \brief A stprage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_STORAGE_HPP)
#define TETENGO_PROPERTY_STORAGE_HPP

#include <memory>

#include <boost/core/noncopyable.hpp>


namespace tetengo::property
{
    /*!
        \brief A storage.
    */
    class storage : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Destroys the storage.
        */
        virtual ~storage();


        // functions


    private:
        // virtual functions
    };


    /*!
        \brief A storage factory.
    */
    class storage_factory : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Destroys the storage factory.
        */
        virtual ~storage_factory();


        // functions

        /*!
            \brief Loads a storage.

            \return A unique pointer to a storage. Or nullptr on error.
        */
        [[nodiscard]] std::unique_ptr<storage> load() const;


    private:
        // virtual functions

        virtual std::unique_ptr<storage> load_impl() const = 0;
    };


}


#endif
