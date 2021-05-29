/*! \file
    \brief A property set.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/property_set.hpp>


namespace tetengo::property
{
    class property_set::impl : private boost::noncopyable
    {
    public:
        // functions
    };


    property_set::property_set() : m_p_impl{ std::make_unique<impl>() } {}

    property_set::~property_set() = default;


}
