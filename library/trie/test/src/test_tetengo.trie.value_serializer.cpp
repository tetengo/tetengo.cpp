/*! \file
    \brief A value serializer.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <functional>
#include <string>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/value_serializer.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(value_serializer)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::value_serializer serializer{ [](const std::any& value) {
                                                             return tetengo::trie::default_serializer<int>{ false }(
                                                                 std::any_cast<int>(value));
                                                         },
                                                          sizeof(int) };
    }
    {
        const tetengo::trie::value_serializer serializer{ [](const std::any&) {
                                                             return std::vector<char>{ 3, 1, 4 };
                                                         },
                                                          0 };
    }
}

BOOST_AUTO_TEST_CASE(operator_paren)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::value_serializer serializer{ [](const std::any& value) {
                                                             return tetengo::trie::default_serializer<int>{ false }(
                                                                 std::any_cast<int>(value));
                                                         },
                                                          sizeof(int) };

        const auto expected = tetengo::trie::default_serializer<int>{ false }(42);
        const auto serialized = serializer(42);
        BOOST_TEST(serialized == expected);
    }
    {
        const tetengo::trie::value_serializer serializer{ [](const std::any&) {
                                                             return std::vector<char>{ 3, 1, 4 };
                                                         },
                                                          0 };

        const auto expected = std::vector<char>{ 3, 1, 4 };
        const auto serialized = serializer("hoge");
        BOOST_TEST(serialized == expected);
    }
}

BOOST_AUTO_TEST_CASE(fixed_value_size)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::value_serializer serializer{ [](const std::any& value) {
                                                             return tetengo::trie::default_serializer<int>{ false }(
                                                                 std::any_cast<int>(value));
                                                         },
                                                          sizeof(int) };

        BOOST_TEST(serializer.fixed_value_size() == sizeof(int));
    }
    {
        const tetengo::trie::value_serializer serializer{ [](const std::any&) {
                                                             return std::vector<char>{ 3, 1, 4 };
                                                         },
                                                          0 };

        BOOST_TEST(serializer.fixed_value_size() == 0U);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(value_deserializer)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            return tetengo::trie::default_deserializer<int>{ false }(serialized);
        } };
    }
    {
        const tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>&) {
            return std::string{ "hoge" };
        } };
    }
}

BOOST_AUTO_TEST_CASE(operator_paren)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            return tetengo::trie::default_deserializer<int>{ false }(serialized);
        } };

        const auto expected = 42;
        const auto serialized = tetengo::trie::default_serializer<int>{ false }(expected);
        const auto deserialized = std::any_cast<int>(deserializer(serialized));
        BOOST_TEST(deserialized == expected);
    }
    {
        const tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>&) {
            return std::string{ "hoge" };
        } };

        const std::string       expected{ "hoge" };
        const std::vector<char> serialized{ 3, 1, 4 };
        const auto              deserialized = std::any_cast<std::string>(deserializer(serialized));
        BOOST_TEST(deserialized == expected);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
