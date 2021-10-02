/*! \file
    \brief A default serializer.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/double_array.hpp>


namespace
{
    struct my_class
    {
        std::vector<char> m_member;
    };

    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    constexpr char nul_byte()
    {
        return 0xFE_c;
    }

}

namespace tetengo::trie
{
    template <typename Object, typename>
    class default_serializer;

    template <>
    class default_serializer<my_class>
    {
    public:
        // functions

        std::vector<char> operator()(const my_class& object) const
        {
            return object.m_member;
        }
    };

    template <typename Object, typename>
    class default_deserializer;

    template <>
    class default_deserializer<my_class>
    {
    public:
        // functions

        my_class operator()(const std::vector<char>& bytes) const
        {
            return my_class{ bytes };
        }
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(default_serializer)


BOOST_AUTO_TEST_CASE(operator_paren)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::default_serializer<std::string_view> serialize{ true };

        const std::string_view object{ "Sakuramachi" };
        const std::string_view expected_serialized{ "Sakuramachi" };
        const auto             serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_TEST(serialized.find(tetengo::trie::double_array::key_terminator()) == std::string::npos);
    }
    {
        const tetengo::trie::default_serializer<std::string> serialize{ true };

        const std::string object{ "Sakuramachi" };
        const std::string expected_serialized{ "Sakuramachi" };
        const auto        serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_TEST(serialized.find(tetengo::trie::double_array::key_terminator()) == std::string::npos);
    }
    {
        const tetengo::trie::default_serializer<std::wstring_view> serialize{ true };

        const std::wstring      object_value{ 0x685C, 0x753A };
        const std::wstring_view object{ object_value };
        const auto              expected_serialized = []() {
            static_assert(sizeof(wchar_t) >= 2);
            std::vector<char> expected(sizeof(wchar_t) * 2, nul_byte());
            expected[sizeof(wchar_t) - 2] = 0x68_c;
            expected[sizeof(wchar_t) - 1] = 0x5C_c;
            expected[sizeof(wchar_t) * 2 - 2] = 0x75_c;
            expected[sizeof(wchar_t) * 2 - 1] = 0x3A_c;
            return expected;
        }();
        const auto serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_CHECK(
            std::find(std::begin(serialized), std::end(serialized), tetengo::trie::double_array::key_terminator()) ==
            std::end(serialized));
    }
    {
        const tetengo::trie::default_serializer<std::wstring> serialize{ true };

        const std::wstring object{ 0x685C, 0x753A };
        const auto         expected_serialized = []() {
            static_assert(sizeof(wchar_t) >= 2);
            std::vector<char> expected(sizeof(wchar_t) * 2, nul_byte());
            expected[sizeof(wchar_t) - 2] = 0x68_c;
            expected[sizeof(wchar_t) - 1] = 0x5C_c;
            expected[sizeof(wchar_t) * 2 - 2] = 0x75_c;
            expected[sizeof(wchar_t) * 2 - 1] = 0x3A_c;
            return expected;
        }();
        const auto serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_CHECK(
            std::find(std::begin(serialized), std::end(serialized), tetengo::trie::double_array::key_terminator()) ==
            std::end(serialized));
    }
    {
        const tetengo::trie::default_serializer<std::int32_t> serialize{ true };

        const auto              object = static_cast<std::int32_t>(0x001234AB);
        const std::vector<char> expected_serialized{ nul_byte(), 0x12_c, 0x34_c, 0xAB_c };
        const auto              serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_CHECK(
            std::find(std::begin(serialized), std::end(serialized), tetengo::trie::double_array::key_terminator()) ==
            std::end(serialized));
    }
    {
        const tetengo::trie::default_serializer<std::int32_t> serialize{ true };

        const auto              object = static_cast<std::int32_t>(0xFCFDFEFF);
        const std::vector<char> expected_serialized{ 0xFC_c, 0xFD_c, 0xFD_c, 0xFD_c, 0xFE_c, 0xFF_c };
        const auto              serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_CHECK(
            std::find(std::begin(serialized), std::end(serialized), tetengo::trie::double_array::key_terminator()) ==
            std::end(serialized));
    }
    {
        const tetengo::trie::default_serializer<my_class> serialize{};

        const my_class          object{ std::vector<char>{ 'h', 'o', 'g', 'e' } };
        const std::vector<char> expected_serialized{ 'h', 'o', 'g', 'e' };
        const auto              serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_CHECK(
            std::find(std::begin(serialized), std::end(serialized), tetengo::trie::double_array::key_terminator()) ==
            std::end(serialized));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(default_deserializer)


BOOST_AUTO_TEST_CASE(operator_paren)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::default_deserializer<std::string> deserialize{ true };

        const std::string serialized{ "Sakuramachi" };
        const std::string expected_object{ "Sakuramachi" };
        const auto        object = deserialize(serialized);
        BOOST_TEST(object == expected_object);
    }
    {
        const tetengo::trie::default_deserializer<std::wstring> deserialize{ true };

        const auto serialized = []() {
            static_assert(sizeof(wchar_t) >= 2);
            std::vector<char> expected(sizeof(wchar_t) * 2, nul_byte());
            expected[sizeof(wchar_t) - 2] = 0x68_c;
            expected[sizeof(wchar_t) - 1] = 0x5C_c;
            expected[sizeof(wchar_t) * 2 - 2] = 0x75_c;
            expected[sizeof(wchar_t) * 2 - 1] = 0x3A_c;
            return expected;
        }();
        const std::wstring expected_object{ 0x685C, 0x753A };
        const auto         object = deserialize(serialized);
        BOOST_CHECK(object == expected_object);
    }
    {
        const tetengo::trie::default_deserializer<std::int32_t> deserialize{ true };

        const std::vector<char> serialized{ nul_byte(), 0x12_c, 0x34_c, 0xAB_c };
        const auto              expected_object = static_cast<std::int32_t>(0x001234AB);
        const auto              object = deserialize(serialized);
        BOOST_TEST(object == expected_object);
    }
    {
        const tetengo::trie::default_deserializer<std::int32_t> deserialize{ true };

        const std::vector<char> serialized{ 0xFC_c, 0xFD_c, 0xFD_c, 0xFD_c, 0xFE_c, 0xFF_c };
        const auto              expected_object = static_cast<std::int32_t>(0xFCFDFEFF);
        const auto              object = deserialize(serialized);
        BOOST_TEST(object == expected_object);
    }
    {
        const tetengo::trie::default_deserializer<my_class> deserialize{};

        const std::vector<char> serialized{ 'h', 'o', 'g', 'e' };
        const my_class          expected_object{ std::vector<char>{ 'h', 'o', 'g', 'e' } };
        const auto              object = deserialize(serialized);
        BOOST_TEST(object.m_member == expected_object.m_member);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
