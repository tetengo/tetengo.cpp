/*! \file
    \brief A Windows registry storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_WINDOWSREGISTRYSTORAGE_HPP)
#define TETENGO_PROPERTY_WINDOWSREGISTRYSTORAGE_HPP

#include <filesystem>
#include <memory>

#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    /*!
        \brief A Windows registry storage.
    */
    class windows_registry_storage : public storage
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a Windows registry storage.

            \param value_map A value map.
            \param path      A path.
        */
        windows_registry_storage(value_map_type value_map, const std::filesystem::path& path);

        /*!
            \brief Destroys the Windows registry storage.
        */
        virtual ~windows_registry_storage();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual void save_impl() const override;
    };


    /*!
        \brief A Windows registry storage loader.
    */
    class windows_registry_storage_loader : public storage_loader
    {
    public:
        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        [[nodiscard]] static const windows_registry_storage_loader& instance();


        // constructors and destructor

        /*!
            \brief Destroys the Windows registry storage loader.
        */
        virtual ~windows_registry_storage_loader();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        windows_registry_storage_loader();


        // virtual functions

        virtual std::unique_ptr<storage> load_impl(const std::filesystem::path&) const override;
    };


}


#endif
