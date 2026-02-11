/*! \file
    \brief A reader iterator.

    $12026 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/stl_interfaces/iterator_interface.hpp> // IWYU pragma: keep
#include <boost/test/unit_test.hpp>

#include <tetengo/json/reader_iterator.hpp>
#include <tetengo/json/stream_reader.hpp>


namespace
{
    const std::string stream_value{ "Shikoku Mannaka" };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(reader_iterator)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::json::reader_iterator iterator{};
    }
    {
        auto                                 p_stream = std::make_unique<std::istringstream>(stream_value);
        tetengo::json::stream_reader         reader{ std::move(p_stream), 10 };
        const tetengo::json::reader_iterator iterator{ reader };
    }
}

BOOST_AUTO_TEST_CASE(dereference)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                                 p_stream = std::make_unique<std::istringstream>(stream_value);
        tetengo::json::stream_reader         reader{ std::move(p_stream), 10 };
        const tetengo::json::reader_iterator iterator{ reader };

        BOOST_TEST(*iterator == 'S');
    }
    {
        const tetengo::json::reader_iterator iterator{};

        BOOST_CHECK_THROW([[maybe_unused]] const auto dereferenced = *iterator, std::logic_error);
    }
}

BOOST_AUTO_TEST_CASE(equal)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::json::reader_iterator iterator1{};
        const tetengo::json::reader_iterator iterator2{};

        BOOST_CHECK(iterator1 == iterator2);
    }
    {
        auto                                 p_stream = std::make_unique<std::istringstream>(stream_value);
        tetengo::json::stream_reader         reader{ std::move(p_stream), 10 };
        const tetengo::json::reader_iterator iterator1{ reader };
        const tetengo::json::reader_iterator iterator2{ iterator1 };

        BOOST_CHECK(iterator1 == iterator2);
    }
    {
        auto                                 p_stream1 = std::make_unique<std::istringstream>(stream_value);
        tetengo::json::stream_reader         reader1{ std::move(p_stream1), 10 };
        const tetengo::json::reader_iterator iterator1{ reader1 };
        auto                                 p_stream2 = std::make_unique<std::istringstream>(stream_value);
        tetengo::json::stream_reader         reader2{ std::move(p_stream2), 10 };
        const tetengo::json::reader_iterator iterator2{ reader2 };

        BOOST_CHECK(iterator1 != iterator2);
    }
    {
        const tetengo::json::reader_iterator iterator1{};
        auto                                 p_stream2 = std::make_unique<std::istringstream>(stream_value);
        tetengo::json::stream_reader         reader2{ std::move(p_stream2), 10 };
        const tetengo::json::reader_iterator iterator2{ reader2 };

        BOOST_CHECK(iterator1 != iterator2);
    }
    {
        auto                                 p_stream1 = std::make_unique<std::istringstream>(stream_value);
        tetengo::json::stream_reader         reader1{ std::move(p_stream1), 10 };
        const tetengo::json::reader_iterator iterator1{ reader1 };
        const tetengo::json::reader_iterator iterator2{};

        BOOST_CHECK(iterator1 != iterator2);
    }
    {
        auto                                 p_stream = std::make_unique<std::istringstream>(stream_value);
        tetengo::json::stream_reader         reader{ std::move(p_stream), 10 };
        tetengo::json::reader_iterator       iterator1{ reader };
        const tetengo::json::reader_iterator iterator2{ iterator1 };

        ++iterator1;

        BOOST_CHECK(iterator1 != iterator2);
    }
}

BOOST_AUTO_TEST_CASE(increment)
{
    BOOST_TEST_PASSPOINT();

    {
        auto                           p_stream = std::make_unique<std::istringstream>(stream_value);
        tetengo::json::stream_reader   reader{ std::move(p_stream), 10 };
        tetengo::json::reader_iterator iterator{ reader };

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'S');
        ++iterator;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'h');
        iterator++;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'i');
        ++iterator;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'k');
        iterator++;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'o');
        ++iterator;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'k');
        iterator++;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'u');
        ++iterator;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == ' ');
        iterator++;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'M');
        ++iterator;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'a');
        iterator++;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'n');
        ++iterator;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'n');
        iterator++;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'a');
        ++iterator;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'k');
        iterator++;

        BOOST_CHECK(iterator != tetengo::json::reader_iterator{});
        BOOST_TEST(*iterator == 'a');
        ++iterator;

        BOOST_CHECK(iterator == tetengo::json::reader_iterator{});
        BOOST_CHECK_THROW([[maybe_unused]] const auto dereferenced = *iterator, std::logic_error);
        BOOST_CHECK_THROW(++iterator, std::logic_error);
    }
    {
        tetengo::json::reader_iterator iterator{};

        BOOST_CHECK_THROW(++iterator, std::logic_error);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
