/*! \file
    \brief A property set.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/property_set.hpp>
#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    class property_set::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::unique_ptr<storage_loader>&& p_storage_loader) :
        m_p_storage_loader{ std::move(p_storage_loader) }
        {}


        // functions


    private:
        // variables

        const std::unique_ptr<storage_loader> m_p_storage_loader;
    };


    property_set::property_set(std::unique_ptr<storage_loader>&& p_storage_loader) :
    m_p_impl{ std::make_unique<impl>(std::move(p_storage_loader)) }
    {}

    property_set::~property_set() = default;


}
