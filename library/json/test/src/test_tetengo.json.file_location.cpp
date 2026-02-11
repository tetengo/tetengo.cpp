/*! \file
    \brief A file location.

    $12026 kaoru  https://www.tetengo.org/
*/

#include <stdexcept>
#include <string>

#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/file_location.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(file_location)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        [[maybe_unused]] const tetengo::json::file_location location_{ "hoge", 42, 4 };
    }
    {
        BOOST_CHECK_THROW(
            [[maybe_unused]] const tetengo::json::file_location location_("hoge", 42, 5), std::out_of_range);
    }
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::json::file_location location1{ "hoge", 42, 4 };
        const tetengo::json::file_location location2{ "hoge", 42, 4 };

        BOOST_CHECK(location1 == location2);
    }
    {
        const tetengo::json::file_location location1{ "hoge", 42, 4 };
        const tetengo::json::file_location location2{ "fuga", 24, 3 };

        BOOST_CHECK(location1 != location2);
    }
}

BOOST_AUTO_TEST_CASE(line)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::json::file_location location_{ "hoge", 42, 4 };

    BOOST_TEST(location_.line() == "hoge");
}

BOOST_AUTO_TEST_CASE(line_index)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::json::file_location location_{ "hoge", 42, 4 };

    BOOST_TEST(location_.line_index() == 42U);
}

BOOST_AUTO_TEST_CASE(column_index)
{
    BOOST_TEST_PASSPOINT();

    const tetengo::json::file_location location_{ "hoge", 42, 4 };

    BOOST_TEST(location_.column_index() == 4U);
}

BOOST_AUTO_TEST_CASE(set_column_index)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::json::file_location location_{ "hoge", 42, 4 };

        location_.set_column_index(2);

        BOOST_TEST(location_.column_index() == 2U);
    }
    {
        tetengo::json::file_location location_{ "hoge", 42, 4 };

        BOOST_CHECK_THROW(location_.set_column_index(5), std::out_of_range);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
