/*! \file
    \brief A Windows registry access.

    For Windows.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/platform_dependent/windows_X.hpp>


namespace tetengo::platform_dependent
{
    class windows_registry::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl() {}

        ~impl() {}


        // functions
    };


    windows_registry::windows_registry() : m_p_impl{ std::make_unique<impl>() } {}

    windows_registry::~windows_registry() = default;


}
