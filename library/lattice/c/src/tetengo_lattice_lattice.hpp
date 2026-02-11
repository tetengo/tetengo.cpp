/*! \file
    \brief A lattice.

    $12026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_LATTICE_HPP_)
#define TETENGO_LATTICE_LATTICE_HPP_

#include <memory>
#include <utility>

#include <tetengo/lattice/lattice.hpp>


struct tetengo_lattice_lattice_tag
{
    std::unique_ptr<tetengo::lattice::lattice> p_cpp_lattice;

    explicit tetengo_lattice_lattice_tag(std::unique_ptr<tetengo::lattice::lattice>&& p_cpp_lattice) :
    p_cpp_lattice{ std::move(p_cpp_lattice) }
    {}
};


#endif
