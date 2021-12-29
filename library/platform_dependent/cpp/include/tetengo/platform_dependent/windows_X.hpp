/*! \file
    \brief A Windows registry access.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PLATFORMDEPENDENT_WINDOWSREGISTRY_HPP)
#define TETENGO_PLATFORMDEPENDENT_WINDOWSREGISTRY_HPP

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <boost/core/noncopyable.hpp>


namespace tetengo::platform_dependent
{
    /*!
        \brief A Windows registry reader.

        This class is only available on Windows.

        The read and write target is fixed to \\HKEY_CURRENT_USER\\SOFTWARE.
    */
    class windows_registry_reader : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a Windows registry reader.

            \param subkey A subkey.
        */
        explicit windows_registry_reader(const std::filesystem::path& subkey);

        /*!
            \brief Destroys the Windows registry reader.
        */
        ~windows_registry_reader();


        // functions

        /*!
            \brief Returns the child subkeys.

            \return The child subkeys.
        */
        std::vector<std::string> child_subkeys() const;

        /*!
            \brief Returns the value name list.

            \return The value name list.
        */
        std::vector<std::string> value_names() const;

        /*!
            \brief Returns the DWORD value.

            \param name A name.

            \return The value.
                    Or std::nullopt when no value is found for the name or the value type is different from DWORD.
        */
        std::optional<std::uint32_t> dword_value_of(const std::string_view& name) const;

        /*!
            \brief Returns the string value.

            \param name A name.

            \return The value.
                    Or std::nullopt when no value is found for the name or the value type is different from string.
        */
        std::optional<std::string> string_value_of(const std::string_view& name) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


    /*!
        \brief A Windows registry writer.

        This class is only available on Windows.

        The read and write target is fixed to \\HKEY_CURRENT_USER\\SOFTWARE.
    */
    class windows_registry_writer : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a Windows registry writer.

            \param subkey A subkey.
        */
        explicit windows_registry_writer(const std::filesystem::path& subkey);

        /*!
            \brief Destroys the Windows registry writer.
        */
        ~windows_registry_writer();


        // functions

        /*!
            \brief Sets a DWORD value.

            \param name  A name.
            \param value A value.
        */
        void set_dword_value_of(const std::string_view& name, std::uint32_t value) const;

        /*!
            \brief Sets a string value.

            \param name  A name.
            \param value A value.
        */
        void set_string_value_of(const std::string_view& name, const std::string_view& value) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
