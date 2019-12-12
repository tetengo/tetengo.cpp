/*!
    \brief A default serializer.

    Copyright (C) 2019 kaoru
 */

#include <cstdint>
#include <string>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/default_serializer.hpp>


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
        const std::string serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
    }
    {
        const tetengo::trie::default_serializer<std::wstring> serialize{};

        const std::wstring object{ 0x685C, 0x753A };
        const std::string  expected_serialized = []() {
            static_assert(sizeof(wchar_t) >= 2);
            std::string expected(sizeof(wchar_t) * 2, 0);
            expected[sizeof(wchar_t) - 2] = to_c(0x68);
            expected[sizeof(wchar_t) - 1] = to_c(0x5C);
            expected[sizeof(wchar_t) * 2 - 2] = to_c(0x75);
            expected[sizeof(wchar_t) * 2 - 1] = to_c(0x3A);
            return expected;
        }();
        const std::string serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
    }
    {
        const tetengo::trie::default_serializer<std::int32_t> serialize{};

        const auto        object = static_cast<std::int32_t>(0x12345678);
        const std::string expected_serialized{ to_c(0x12), to_c(0x34), to_c(0x56), to_c(0x78) };
        const std::string serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
    }
    {
        const tetengo::trie::default_serializer<my_class> serialize{};

        const my_class    object{ "hoge" };
        const std::string expected_serialized{ "hoge" };
        const std::string serialized = serialize(object);
        BOOST_TEST(serialized == expected_serialized);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
