/*! \file
    \brief A channel.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#include <exception>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/channel.hpp>
#include <tetengo/json/element.hpp>
#include <tetengo/json/file_location.hpp>


namespace
{
    using element_type = tetengo::json::element;


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(channel)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::json::channel channel_{ 42 };
    }
    {
        BOOST_CHECK_THROW(const tetengo::json::channel channel_{ 0 }, std::invalid_argument);
    }
}

BOOST_AUTO_TEST_CASE(insert)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::json::channel channel_{ 42 };

        element_type element{ element_type::type_type{ element_type::type_name_type::string,
                                                       element_type::type_category_type::primitive },
                              "tateno",
                              std::unordered_map<std::string, std::string>{},
                              tetengo::json::file_location{ "hoge", 2, 3 } };
        channel_.insert(std::move(element));
    }
    {
        tetengo::json::channel channel_{ 42 };

        channel_.insert(std::make_exception_ptr(std::runtime_error{ "seta" }));
    }
}

BOOST_AUTO_TEST_CASE(peek)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::json::channel channel_{ 42 };

        element_type element{ element_type::type_type{ element_type::type_name_type::string,
                                                       element_type::type_category_type::primitive },
                              "tateno",
                              std::unordered_map<std::string, std::string>{},
                              tetengo::json::file_location{ "hoge", 2, 3 } };
        channel_.insert(std::move(element));

        const auto& peeked = channel_.peek();
        BOOST_CHECK(peeked.type().name == element_type::type_name_type::string);
        BOOST_TEST(peeked.value() == "tateno");
    }
    {
        tetengo::json::channel channel_{ 42 };

        channel_.insert(std::make_exception_ptr(std::runtime_error{ "seta" }));

        BOOST_CHECK_THROW([[maybe_unused]] const auto& peeked = channel_.peek(), std::runtime_error);
    }
}

BOOST_AUTO_TEST_CASE(take)
{
    BOOST_TEST_PASSPOINT();

    tetengo::json::channel channel_{ 2 };

    std::thread inserter{ [&channel_]() {
        channel_.insert(element_type{ element_type::type_type{ element_type::type_name_type::string,
                                                               element_type::type_category_type::primitive },
                                      "tateno",
                                      std::unordered_map<std::string, std::string>{},
                                      tetengo::json::file_location{ "hoge", 2, 3 } });
        channel_.insert(element_type{ element_type::type_type{ element_type::type_name_type::string,
                                                               element_type::type_category_type::primitive },
                                      "akamizu",
                                      std::unordered_map<std::string, std::string>{},
                                      tetengo::json::file_location{ "hoge", 2, 3 } });
        channel_.insert(element_type{ element_type::type_type{ element_type::type_name_type::string,
                                                               element_type::type_category_type::primitive },
                                      "ichinokawa",
                                      std::unordered_map<std::string, std::string>{},
                                      tetengo::json::file_location{ "hoge", 2, 3 } });
        channel_.insert(element_type{ element_type::type_type{ element_type::type_name_type::string,
                                                               element_type::type_category_type::primitive },
                                      "uchinomaki",
                                      std::unordered_map<std::string, std::string>{},
                                      tetengo::json::file_location{ "hoge", 2, 3 } });
        channel_.insert(element_type{ element_type::type_type{ element_type::type_name_type::string,
                                                               element_type::type_category_type::primitive },
                                      "aso",
                                      std::unordered_map<std::string, std::string>{},
                                      tetengo::json::file_location{ "hoge", 2, 3 } });
    } };

    BOOST_TEST(channel_.peek().value() == "tateno");
    channel_.take();
    BOOST_TEST(channel_.peek().value() == "akamizu");
    channel_.take();
    BOOST_TEST(channel_.peek().value() == "ichinokawa");
    channel_.take();
    BOOST_TEST(channel_.peek().value() == "uchinomaki");
    channel_.take();
    BOOST_TEST(channel_.peek().value() == "aso");
    channel_.take();

    inserter.join();
}

BOOST_AUTO_TEST_CASE(closed)
{
    BOOST_TEST_PASSPOINT();

    tetengo::json::channel channel_{ 42 };

    channel_.insert(element_type{
        element_type::type_type{ element_type::type_name_type::string, element_type::type_category_type::primitive },
        "tateno",
        std::unordered_map<std::string, std::string>{},
        tetengo::json::file_location{ "hoge", 2, 3 } });

    BOOST_TEST(!channel_.closed());
}

BOOST_AUTO_TEST_CASE(close)
{
    BOOST_TEST_PASSPOINT();

    tetengo::json::channel channel_{ 42 };

    channel_.close();

    BOOST_TEST(channel_.closed());

    channel_.insert(element_type{
        element_type::type_type{ element_type::type_name_type::string, element_type::type_category_type::primitive },
        "tateno",
        std::unordered_map<std::string, std::string>{},
        tetengo::json::file_location{ "hoge", 2, 3 } });

    BOOST_CHECK_THROW([[maybe_unused]] const auto& peeked = channel_.peek(), std::logic_error);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
