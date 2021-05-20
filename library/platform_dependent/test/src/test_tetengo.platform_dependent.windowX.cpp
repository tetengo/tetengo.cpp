/*! \file
    \brief A Windows registry access.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if _WIN32

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/platform_dependent/windows_X.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(platform_dependent)
BOOST_AUTO_TEST_SUITE(windows_registry)


BOOST_AUTO_TEST_CASE(instance)
{
    BOOST_TEST_PASSPOINT();

    [[maybe_unused]] const auto& registry = tetengo::platform_dependent::windows_registry::instance();
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

#endif
