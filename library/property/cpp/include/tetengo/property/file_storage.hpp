/*! \file
    \brief A file storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_FILESTORAGE_HPP)
#define TETENGO_PROPERTY_FILESTORAGE_HPP

#include <filesystem>
#include <memory>

#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    /*!
        \brief A file storage.
    */
    class file_storage : public storage
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a file storage.

            \param value_map  A value map.
        */
        explicit file_storage(value_map_type value_map);

        /*!
            \brief Destroys the file storage.
        */
        virtual ~file_storage();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual void save_impl() const override;
    };


    /*!
        \brief A file storage loader.

        The storage path specified for the member function load() is not used and ignored.
    */
    class file_storage_loader : public storage_loader
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a file storage loader.
        */
        file_storage_loader();

        /*!
            \brief Destroys the file storage loader.
        */
        virtual ~file_storage_loader();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual std::unique_ptr<storage> load_impl(const std::filesystem::path& path) const override;
    };


}


#endif
