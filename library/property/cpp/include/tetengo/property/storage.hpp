/*! \file
    \brief A stprage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_STORAGE_HPP)
#define TETENGO_PROPERTY_STORAGE_HPP

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>

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

        /*!
            \brief Returns the value in an unsigned 32-bit integer.

            \param key A key.

            \return The value. Or std::nullopt when no such key.
        */
        std::optional<std::uint32_t> get_uint32(const std::filesystem::path& key) const;

        /*!
            \brief Sets a value in an unsigned 32-bit integer.

            \param key   A key.
            \param value A value.
        */
        void set_uint32(const std::filesystem::path& key, std::uint32_t value);

        /*!
            \brief Saves the values.
        */
        void save() const;


    private:
        // virtual functions

        virtual std::optional<std::uint32_t> get_uint32_impl(const std::filesystem::path& key) const = 0;

        virtual void set_uint32_impl(const std::filesystem::path& key, std::uint32_t value) = 0;

        virtual void save_impl() const = 0;
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
