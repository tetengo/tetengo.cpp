/*! \file
    \brief A stream reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/stream_reader.hpp>


namespace
{
    const std::string stream_value{ "Shikoku Mannaka" };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(stream_reader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                               p_stream = std::make_unique<std::istringstream>(stream_value);
        const tetengo::json::stream_reader reader{ std::move(p_stream), 10 };
    }
}

BOOST_AUTO_TEST_CASE(has_next)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                               p_stream = std::make_unique<std::istringstream>(stream_value);
        const tetengo::json::stream_reader reader{ std::move(p_stream), 10 };

        BOOST_TEST(reader.has_next());
    }
}

BOOST_AUTO_TEST_CASE(get)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                               p_stream = std::make_unique<std::istringstream>(stream_value);
        const tetengo::json::stream_reader reader{ std::move(p_stream), 10 };

        BOOST_TEST(reader.get() == 'S');
    }
}

BOOST_AUTO_TEST_CASE(next)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                         p_stream = std::make_unique<std::istringstream>(stream_value);
        tetengo::json::stream_reader reader{ std::move(p_stream), 10 };

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'S');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'h');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'i');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'k');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'o');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'k');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'u');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == ' ');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'M');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'a');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'n');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'n');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'a');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'k');
        reader.next();

        BOOST_TEST_REQUIRE(reader.has_next());
        BOOST_TEST(reader.get() == 'a');
        reader.next();

        BOOST_TEST(!reader.has_next());
        BOOST_CHECK_THROW(reader.get(), std::logic_error);
        BOOST_CHECK_THROW(reader.next(), std::logic_error);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
