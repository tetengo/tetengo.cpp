/*! \file
    \brief A text encoder.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/platform_dependent/text_encoder.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(platform_dependent)
BOOST_AUTO_TEST_SUITE(text_encoder)


BOOST_AUTO_TEST_CASE(instance)
{
    BOOST_TEST_PASSPOINT();

    tetengo::platform_dependent::text_encoder::instance();
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
