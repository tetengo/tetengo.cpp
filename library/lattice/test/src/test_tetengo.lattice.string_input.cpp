/*! \file
    \brief A string input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <string>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/string_input.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(string_input)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::lattice::string_input input{ "hoge" };
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::string_input input{ "hoge" };

        BOOST_TEST(input.value() == "hoge");
    }
}

BOOST_AUTO_TEST_CASE(length)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::lattice::string_input input{ "hoge" };

    BOOST_TEST(input.length() == 4U);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
