/*!
    \brief A trie iterator.

    Copyright (C) 2019 kaoru
 */

#include <iterator>
#include <string>
#include <utility>

#include <boost/core/ignore_unused.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/trie_iterator.hpp>


namespace
{
    constexpr char to_c(const unsigned char uc)
    {
        return static_cast<char>(uc);
    }

    static const std::string kumamoto1{
        to_c(0xE7), to_c(0x86), to_c(0x8A), to_c(0x6), to_c(0x9C), to_c(0xAC) // Kumamoto in Kanji in UTF-8
    };

    static const std::string tamana1{
        to_c(0xE7), to_c(0x8E), to_c(0x89), to_c(0xE5), to_c(0x90), to_c(0x8D) // Tamana in Kanji in UTF-8
    };

    static const std::wstring kumamoto2{ 0x718A, 0x672C }; // Kumamoto in Kanji in UTF-16/32

    static const std::wstring tamana2{ 0x7389, 0x540D }; // Tamana in Kanji in UTF-16/32


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(trie_iterator)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::trie::trie_iterator_impl                             impl{};
        const tetengo::trie::trie_iterator<std::wstring, std::string> iterator{ std::move(impl) };
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{};

        boost::ignore_unused(std::begin(trie_));
        boost::ignore_unused(std::end(trie_));
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 }, { tamana2, tamana1 } };

        boost::ignore_unused(std::begin(trie_));
        boost::ignore_unused(std::end(trie_));
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(operator_dereference)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}

BOOST_AUTO_TEST_CASE(increment)
{
    BOOST_TEST_PASSPOINT();

    BOOST_WARN_MESSAGE(false, "Implement it.");
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
