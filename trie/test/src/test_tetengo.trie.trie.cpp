/*!
    \brief A trie.

    Copyright (C) 2019 kaoru
 */

#include <cstddef>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/trie.hpp>


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
BOOST_AUTO_TEST_SUITE(trie)


BOOST_AUTO_TEST_CASE(null_building_observer_set)
{
    BOOST_TEST_PASSPOINT();

    tetengo::trie::trie<std::string, int>::null_building_observer_set();
}

BOOST_AUTO_TEST_CASE(default_double_array_density_factor)
{
    BOOST_TEST_PASSPOINT();

    using trie_type = tetengo::trie::trie<std::string, int>;
    BOOST_TEST(
        trie_type::default_double_array_density_factor() == tetengo::trie::double_array::default_density_factor());
}

BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::trie<std::string, int> trie_{};
    }
    {
        const tetengo::trie::trie<std::string, int> trie_{ { "Kumamoto", 42 }, { "Tamana", 24 } };
    }
    {
        const tetengo::trie::trie<std::string, int> trie_{ { kumamoto1, 42 }, { tamana1, 24 } };
    }
    {
        const tetengo::trie::trie<std::wstring, std::string> trie_{ { kumamoto2, kumamoto1 }, { tamana2, tamana1 } };
    }
    {
        std::vector<std::string>                                          added_serialized_keys{};
        auto                                                              done = false;
        tetengo::trie::trie<std::string, int>::building_observer_set_type observer{
            [&added_serialized_keys](const std::string_view& serialized_key) {
                added_serialized_keys.emplace_back(serialized_key);
            },
            [&done]() { done = true; }
        };
        const tetengo::trie::trie<std::string, int> trie_{
            { { "Kumamoto", 42 }, { "Tamana", 24 } },
            tetengo::trie::default_serializer<std::string>{},
            observer,
            tetengo::trie::trie<std::string, int>::default_double_array_density_factor()
        };

        static const tetengo::trie::default_deserializer<std::string> key_deserializer{};
        BOOST_TEST_REQUIRE(added_serialized_keys.size() == 2U);
        BOOST_TEST(key_deserializer(added_serialized_keys[0]) == "Kumamoto");
        BOOST_TEST(key_deserializer(added_serialized_keys[1]) == "Tamana");
        BOOST_TEST(done);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
