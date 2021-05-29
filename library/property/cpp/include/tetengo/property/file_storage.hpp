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

            \param value_map A value map.
            \param path      A path.
        */
        file_storage(value_map_type value_map, const std::filesystem::path& path);

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
        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        [[nodiscard]] static const file_storage_loader& instance();


        // constructors and destructor

        /*!
            \brief Destroys the file storage loader.
        */
        virtual ~file_storage_loader();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        file_storage_loader();


        // virtual functions

        virtual std::unique_ptr<storage> load_impl(const std::filesystem::path& path) const override;
    };


}


#endif
