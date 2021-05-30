/*! \file
    \brief A storage proxy.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_STORAGEPROXY_HPP)
#define TETENGO_PROPERTY_STORAGEPROXY_HPP

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    /*!
        \brief A storage proxy.
    */
    class storage_proxy : public storage
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a storage proxy.

            \param p_real_storage_loader A shared pointer to a real storage loader.
            \param path                  A path.
        */
        storage_proxy(std::shared_ptr<storage_loader> p_real_storage_loader, const std::filesystem::path& path);

        /*!
            \brief Destroys the storage proxy.
        */
        virtual ~storage_proxy();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual std::optional<bool> get_bool_impl(const std::filesystem::path& key) const override;

        virtual void set_bool_impl(const std::filesystem::path& key, bool value) override;

        virtual std::optional<std::uint32_t> get_uint32_impl(const std::filesystem::path& key) const override;

        virtual void set_uint32_impl(const std::filesystem::path& key, std::uint32_t value) override;

        virtual std::optional<std::string> get_string_impl(const std::filesystem::path& key) const override;

        virtual void set_string_impl(const std::filesystem::path& key, const std::string& value) override;

        virtual void save_impl() const override;
    };


    /*!
        \brief A storage proxy loader.
    */
    class storage_proxy_loader : public storage_loader
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a storage proxy loader.

            \param p_real_storage_loader A unique pointer to a real storage loader.
        */
        explicit storage_proxy_loader(std::unique_ptr<storage_loader>&& p_real_storage_loader);

        /*!
            \brief Destroys the storage proxy loader.
        */
        virtual ~storage_proxy_loader();


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
