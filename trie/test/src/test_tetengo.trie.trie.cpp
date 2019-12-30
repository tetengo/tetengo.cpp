/*!
    \brief A trie.

    Copyright (C) 2019 kaoru
 */

#include <functional>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/storage.hpp>
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

    const std::vector<char> serialized{
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x02), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x2A), to_c(0xFF), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x18), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x03), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x04), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x02), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x01), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x03), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x04), /*                                                            */
        to_c(0x68), to_c(0x6F), to_c(0x67), to_c(0x65), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x04), /*                                                            */
        to_c(0x66), to_c(0x75), to_c(0x67), to_c(0x61), /*                                                            */
        to_c(0x00), to_c(0x00), to_c(0x00), to_c(0x04), /*                                                            */
        to_c(0x70), to_c(0x69), to_c(0x79), to_c(0x6F), /*                                                            */
    };

    std::unique_ptr<std::istream> create_input_stream()
    {
        return std::make_unique<std::stringstream>(std::string{ std::begin(serialized), std::end(serialized) });
    }

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
    {
        auto p_input_stream = create_input_stream();
        auto p_storage =
            std::make_unique<tetengo::trie::memory_storage>(*p_input_stream, [](const std::string_view& serialized) {
                static const tetengo::trie::default_deserializer<std::string> string_deserializer{};
                return string_deserializer(std::string{ serialized });
            });
        const tetengo::trie::trie<std::string, std::string> trie_{ std::move(p_storage) };
    }
}

BOOST_AUTO_TEST_CASE(get_storage)
{
    BOOST_TEST_PASSPOINT();

    {
        auto p_input_stream = create_input_stream();
        auto p_storage =
            std::make_unique<tetengo::trie::memory_storage>(*p_input_stream, [](const std::string_view& serialized) {
                static const tetengo::trie::default_deserializer<std::string> string_deserializer{};
                return string_deserializer(std::string{ serialized });
            });
        const tetengo::trie::trie<std::string, std::string> trie_{ std::move(p_storage) };

        trie_.get_storage();
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
