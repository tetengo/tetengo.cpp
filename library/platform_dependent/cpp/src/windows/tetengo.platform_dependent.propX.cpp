/*! \file
    \brief A property set file path.

    For Windows.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/platform_dependent/propertyX.hpp>


namespace tetengo::platform_dependent
{
    class property_set_file_path::impl : private boost::noncopyable
    {
    public:
        // static functions

        static const property_set_file_path& instance()
        {
            static const property_set_file_path singleton{};
            return singleton;
        }


        // functions
    };


    const property_set_file_path& property_set_file_path::instance()
    {
        return impl::instance();
    }

    property_set_file_path::~property_set_file_path() = default;

    property_set_file_path::property_set_file_path() : m_p_impl{ std::make_unique<impl>() } {}
}
