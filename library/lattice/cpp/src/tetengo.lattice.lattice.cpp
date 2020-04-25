/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/lattice/lattice.hpp>


namespace tetengo::lattice
{
    class lattice::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl() {}
    };


    lattice::lattice() : m_p_impl{ std::make_unique<impl>() } {}

    lattice::~lattice() = default;
}
