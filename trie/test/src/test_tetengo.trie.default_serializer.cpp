/*!
    \brief A default serializer.

    Copyright (C) 2019 kaoru
 */

#include <cstdint>
#include <string>
#include <string_view>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/double_array.hpp>


namespace
{
    struct my_class
    {
        std::string m_member;
    };

    constexpr char to_c(const unsigned char uc)
    {
        return static_cast<char>(uc);
    }

    constexpr char nul_byte()
    {
        return to_c(0xFE);
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

        std::string operator()(const my_class& object) const
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

        my_class operator()(const std::string_view& bytes) const
        {
            return my_class{ std::string{ bytes } };
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
        const tetengo::trie::default_serializer<std::string> serialize{};

        const std::string object{ "Sakuramachi" };
        const std::string expected_serialized{ "Sakuramachi" };
        const auto        serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_TEST(serialized.find(tetengo::trie::double_array::key_terminator()) == std::string::npos);
    }
    {
        const tetengo::trie::default_serializer<std::string_view> serialize{};

        const std::string_view object{ "Sakuramachi" };
        const std::string_view expected_serialized{ "Sakuramachi" };
        const auto        serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_TEST(serialized.find(tetengo::trie::double_array::key_terminator()) == std::string::npos);
    }
    {
        const tetengo::trie::default_serializer<std::wstring> serialize{};

        const std::wstring object{ 0x685C, 0x753A };
        const std::string  expected_serialized = []() {
            static_assert(sizeof(wchar_t) >= 2);
            std::string expected(sizeof(wchar_t) * 2, nul_byte());
            expected[sizeof(wchar_t) - 2] = to_c(0x68);
            expected[sizeof(wchar_t) - 1] = to_c(0x5C);
            expected[sizeof(wchar_t) * 2 - 2] = to_c(0x75);
            expected[sizeof(wchar_t) * 2 - 1] = to_c(0x3A);
            return expected;
        }();
        const auto serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_TEST(serialized.find(tetengo::trie::double_array::key_terminator()) == std::string::npos);
    }
    {
        const tetengo::trie::default_serializer<std::int32_t> serialize{};

        const auto        object = static_cast<std::int32_t>(0x001234AB);
        const std::string expected_serialized{ nul_byte(), to_c(0x12), to_c(0x34), to_c(0xAB) };
        const auto        serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_TEST(serialized.find(tetengo::trie::double_array::key_terminator()) == std::string::npos);
    }
    {
        const tetengo::trie::default_serializer<std::int32_t> serialize{};

        const auto        object = static_cast<std::int32_t>(0xFCFDFEFF);
        const std::string expected_serialized{ to_c(0xFC), to_c(0xFD), to_c(0xFD), to_c(0xFD), to_c(0xFE), to_c(0xFF) };
        const auto        serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_TEST(serialized.find(tetengo::trie::double_array::key_terminator()) == std::string::npos);
    }
    {
        const tetengo::trie::default_serializer<my_class> serialize{};

        const my_class    object{ "hoge" };
        const std::string expected_serialized{ "hoge" };
        const auto        serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
        BOOST_TEST(serialized.find(tetengo::trie::double_array::key_terminator()) == std::string::npos);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(default_deserializer)


BOOST_AUTO_TEST_CASE(operator_paren)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::default_deserializer<std::string> deserialize{};

        const std::string serialized{ "Sakuramachi" };
        const std::string expected_object{ "Sakuramachi" };
        const auto        object = deserialize(serialized);
        BOOST_TEST(object == expected_object);
    }
    {
        const tetengo::trie::default_deserializer<std::wstring> deserialize{};

        const std::string serialized = []() {
            static_assert(sizeof(wchar_t) >= 2);
            std::string expected(sizeof(wchar_t) * 2, nul_byte());
            expected[sizeof(wchar_t) - 2] = to_c(0x68);
            expected[sizeof(wchar_t) - 1] = to_c(0x5C);
            expected[sizeof(wchar_t) * 2 - 2] = to_c(0x75);
            expected[sizeof(wchar_t) * 2 - 1] = to_c(0x3A);
            return expected;
        }();
        const std::wstring expected_object{ 0x685C, 0x753A };
        const auto         object = deserialize(serialized);
        BOOST_CHECK(object == expected_object);
    }
    {
        const tetengo::trie::default_deserializer<std::int32_t> deserialize{};

        const std::string serialized{ nul_byte(), to_c(0x12), to_c(0x34), to_c(0xAB) };
        const auto        expected_object = static_cast<std::int32_t>(0x001234AB);
        const auto        object = deserialize(serialized);
        BOOST_TEST(object == expected_object);
    }
    {
        const tetengo::trie::default_deserializer<std::int32_t> deserialize{};

        const std::string serialized{ to_c(0xFC), to_c(0xFD), to_c(0xFD), to_c(0xFD), to_c(0xFE), to_c(0xFF) };
        const auto        expected_object = static_cast<std::int32_t>(0xFCFDFEFF);
        const auto        object = deserialize(serialized);
        BOOST_TEST(object == expected_object);
    }
    {
        const tetengo::trie::default_deserializer<my_class> deserialize{};

        const std::string serialized{ "hoge" };
        const my_class    expected_object{ "hoge" };
        const auto        object = deserialize(serialized);
        BOOST_TEST(object.m_member == expected_object.m_member);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
