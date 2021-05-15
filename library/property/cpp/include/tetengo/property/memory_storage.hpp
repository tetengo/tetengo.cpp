/*! \file
    \brief A memory storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_MEMORYSTORAGE_HPP)
#define TETENGO_PROPERTY_MEMORYSTORAGE_HPP

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>

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
            \brief Creates a memory storage.
        */
        memory_storage();

        /*!
            \brief Destroys the memory storage.
        */
        virtual ~memory_storage();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual std::optional<std::uint32_t> get_uint32_impl(const std::filesystem::path& key) const override;

        virtual void set_uint32_impl(const std::filesystem::path& key, std::uint32_t value) override;

        virtual void save_impl() const override;
    };


    /*!
        \brief A memory storage loader.

        The storage path specified for the member function load() is not used and ignored.
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
