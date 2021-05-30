/*! \file
    \brief A property set.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_PROPERTYSET_HPP)
#define TETENGO_PROPERTY_PROPERTYSET_HPP

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

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
            \param path             A path.
        */
        property_set(std::unique_ptr<storage_loader>&& p_storage_loader, const std::filesystem::path& path);

        /*!
            \brief Destroys the property set.
        */
        virtual ~property_set();


        // functions

        /*!
            \brief Returns the value in a boolean.

            \param key A key.

            \return The value. Or std::nullopt when no such key.
        */
        [[nodiscard]] std::optional<bool> get_bool(const std::filesystem::path& key) const;

        /*!
            \brief Sets a value in a boolean.

            \param key   A key.
            \param value A value.
        */
        void set_bool(const std::filesystem::path& key, bool value);

        /*!
            \brief Returns the value in an unsigned 32-bit integer.

            \param key A key.

            \return The value. Or std::nullopt when no such key.
        */
        [[nodiscard]] std::optional<std::uint32_t> get_uint32(const std::filesystem::path& key) const;

        /*!
            \brief Sets a value in an unsigned 32-bit integer.

            \param key   A key.
            \param value A value.
        */
        void set_uint32(const std::filesystem::path& key, std::uint32_t value);

        /*!
            \brief Returns the value in a string.

            \param key A key.

            \return The value. Or std::nullopt when no such key.
        */
        [[nodiscard]] std::optional<std::string> get_string(const std::filesystem::path& key) const;

        /*!
            \brief Sets a value in a string.

            \param key   A key.
            \param value A value.
        */
        void set_string(const std::filesystem::path& key, const std::string& value);

        /*!
            \brief Updates the property values to the latest state in the storage.
        */
        void update();

        /*!
            \brief Commits the property value changes to the storage.
        */
        void commit() const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions
    };


}


#endif
