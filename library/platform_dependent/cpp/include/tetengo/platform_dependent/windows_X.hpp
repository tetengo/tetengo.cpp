/*! \file
    \brief A Windows registry access.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PLATFORMDEPENDENT_WINDOWSREGISTRY_HPP)
#define TETENGO_PLATFORMDEPENDENT_WINDOWSREGISTRY_HPP

#include <filesystem>
#include <memory>

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


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
