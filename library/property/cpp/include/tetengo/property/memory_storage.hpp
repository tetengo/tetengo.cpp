/*! \file
    \brief A memory storage.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_MEMORYSTORAGE_HPP)
#define TETENGO_PROPERTY_MEMORYSTORAGE_HPP

#include <filesystem>
#include <memory>

#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    /*!
        \brief A memory storage.
    */
    class memory_storage : public storage
    {
    public:
        // constructors and destructor

        /*!
            \brief Destroys the memory storage.
        */
        virtual ~memory_storage();


    private:
        // types

        class impl;

        friend class memory_storage_loader;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        explicit memory_storage(value_map_type& master_value_map);


        // virtual functions

        virtual void save_impl() const override;
    };


    /*!
        \brief A memory storage loader.
    */
    class memory_storage_loader : public storage_loader
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a memory storage loader.
        */
        memory_storage_loader();

        /*!
            \brief Destroys the memory storage loader.
        */
        virtual ~memory_storage_loader();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual std::unique_ptr<storage> load_impl(const std::filesystem::path&) const override;
    };


}


#endif
