/*! \file
    \brief The usage test of tetengo::lattice.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/


#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include "usage_tetengo.lattice.viterbi_c.h"
#include "usage_tetengo.lattice.viterbi_cpp.hpp"


BOOST_AUTO_TEST_SUITE(usagetest_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)


BOOST_AUTO_TEST_CASE(viterbi)
{
    BOOST_TEST_PASSPOINT();

    usage_tetengo::lattice::viterbi();
    usage_tetengo_lattice_viterbi();
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
