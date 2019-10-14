/*!
    \brief An enumerator.

    Copyright (C) 2019 kaoru
 */

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/enumerator.hpp>


namespace
{
    constexpr char to_c(const unsigned char uc)
    {
        return static_cast<char>(uc);
    }

    const std::vector<std::pair<std::string, std::int32_t>> expected_values{ { "UTIGOSI", 24 },
                                                                             { "UTO", 2424 },
                                                                             { "SETA", 42 } };

    const std::vector<std::pair<std::string, std::int32_t>> expected_values2{
        { { to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE7), to_c(0x80), to_c(0xAC) }, 24 }, // "Akase" in Kanji
        { { to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE6), to_c(0xB0), to_c(0xB4) }, 42 }, // "Akamizu" in Kanji
    };

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(enumerator)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};
        const auto                        enumerator = double_array_.get_enumerator();
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values };
        const auto                        enumerator = double_array_.get_enumerator();
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values2 };
        const auto                        enumerator = double_array_.get_enumerator();
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(next)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};
        const auto                        enumerator = double_array_.get_enumerator();

        {
            const auto element = enumerator.next();

            BOOST_CHECK(!element);
        }
        {
            const auto element = enumerator.next();

            BOOST_CHECK(!element);
        }
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values };
        const auto                        enumerator = double_array_.get_enumerator();

        {
            const auto element = enumerator.next();

            BOOST_REQUIRE(element);
            BOOST_TEST(element->first == "SETA");
            BOOST_TEST(element->second == 42);
        }
        {
            const auto element = enumerator.next();

            BOOST_REQUIRE(element);
            BOOST_TEST(element->first == "UTIGOSI");
            BOOST_TEST(element->second == 24);
        }
        {
            const auto element = enumerator.next();

            BOOST_REQUIRE(element);
            BOOST_TEST(element->first == "UTO");
            BOOST_TEST(element->second == 2424);
        }
        {
            const auto element = enumerator.next();

            BOOST_CHECK(!element);
        }
        {
            const auto element = enumerator.next();

            BOOST_CHECK(!element);
        }
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values2 };
        const auto                        enumerator = double_array_.get_enumerator();

        {
            const auto element = enumerator.next();

            BOOST_REQUIRE(element);
            const std::string expected_key{ to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE6), to_c(0xB0), to_c(0xB4) };
            BOOST_TEST(element->first == expected_key);
            BOOST_TEST(element->second == 42);
        }
        {
            const auto element = enumerator.next();

            BOOST_REQUIRE(element);
            const std::string expected_key{ to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE7), to_c(0x80), to_c(0xAC) };
            BOOST_TEST(element->first == expected_key);
            BOOST_TEST(element->second == 24);
        }
        {
            const auto element = enumerator.next();

            BOOST_CHECK(!element);
        }
        {
            const auto element = enumerator.next();

            BOOST_CHECK(!element);
        }
    }
    {
        // TODO: C style API
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
