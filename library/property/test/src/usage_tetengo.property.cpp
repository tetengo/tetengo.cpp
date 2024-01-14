/*! \file
    \brief The usage test of tetengo::property.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/


#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include "usage_tetengo.property.saveLoad_c.h"
#include "usage_tetengo.property.save_load_cpp.hpp"


BOOST_AUTO_TEST_SUITE(usagetest_tetengo)
BOOST_AUTO_TEST_SUITE(property)


BOOST_AUTO_TEST_CASE(save_load)
{
    BOOST_TEST_PASSPOINT();

    usage_tetengo::property::save_load();
    usage_tetengo_property_saveLoad();
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
