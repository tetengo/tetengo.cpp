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
        \brief A Windows registry access.

        This class is only available on Windows.

        The read and write target is fixed to \\HKEY_CURRENT_USER\\SOFTWARE.
    */
    class windows_registry : private boost::noncopyable
    {
    public:
        // types

        //! The open mode type.
        enum class open_mode_type
        {
            read, //!< Read.
            write, //!< Write.
        };


        // constructors and destructor

        /*!
            \brief Creates a Windows registry access.

            \param subkey    A subkey.
            \param open_mode An open mode.
        */
        windows_registry(const std::filesystem::path& subkey, open_mode_type open_mode);

        /*!
            \brief Destroys the Windows registry access.
        */
        ~windows_registry();


        // functions


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
