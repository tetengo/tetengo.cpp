/*!
    \brief A default key serializer.

    Copyright (C) 2019 kaoru
 */

#include <string>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/default_key_serializer.hpp>


namespace tetengo::trie
{
    template <typename Key>
    class default_key_serializer;

    template <typename Char>
    class default_key_serializer<std::vector<Char>>
    {
    public:
        // functions

        std::string operator()(const std::vector<Char>& key) const
        {
            return std::string{ key.begin(), key.end() };
        }
    };


}


namespace
{
    constexpr char to_c(const unsigned char uc)
    {
        return static_cast<char>(uc);
    }

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(default_key_serializer)


BOOST_AUTO_TEST_CASE(operator_paren)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::default_key_serializer<std::string> serialize{};

        const std::string key{ "Sakuramachi" };
        const std::string expected_serialized{ "Sakuramachi" };
        const std::string serialized = serialize(key);
        BOOST_TEST(serialized == expected_serialized);
    }
    {
        const tetengo::trie::default_key_serializer<std::wstring> serialize{};

        const std::wstring key{ 0x685C, 0x753A };
        const std::string  expected_serialized = []() {
            static_assert(sizeof(wchar_t) >= 2);
            std::string expected(sizeof(wchar_t) * 2, 0);
            expected[sizeof(wchar_t) - 2] = to_c(0x68);
            expected[sizeof(wchar_t) - 1] = to_c(0x5C);
            expected[sizeof(wchar_t) * 2 - 2] = to_c(0x75);
            expected[sizeof(wchar_t) * 2 - 1] = to_c(0x3A);
            return expected;
        }();
        const std::string serialized = serialize(key);
        BOOST_TEST(serialized == expected_serialized);
    }
    {
        const tetengo::trie::default_key_serializer<std::vector<char>> serialize{};

        const std::vector<char> key{ 'T', 'o', 'r', 'i', 'c', 'h', 'o', 's', 'u', 'j', 'i' };
        const std::string       expected_serialized{ "Torichosuji" };
        const std::string       serialized = serialize(key);
        BOOST_TEST(serialized == expected_serialized);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
