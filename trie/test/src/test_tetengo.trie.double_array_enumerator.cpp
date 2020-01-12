/*!
    \brief A double array enumerator.

    Copyright (C) 2019 kaoru
 */

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <boost/core/ignore_unused.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/double_array_enumerator.hpp>


namespace
{
    constexpr char to_c(const unsigned char uc)
    {
        return static_cast<char>(uc);
    }

    const std::vector<std::pair<std::string, std::int32_t>> expected_base_values{ { "UTIGOSI", 24 },
                                                                                  { "UTO", 2424 },
                                                                                  { "SETA", 42 } };

    const std::vector<std::pair<std::string, std::int32_t>> expected_base_values2{
        { { to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE7), to_c(0x80), to_c(0xAC) }, 24 }, // "Akase" in Kanji
        { { to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE6), to_c(0xB0), to_c(0xB4) }, 42 }, // "Akamizu" in Kanji
    };

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(double_array_enumerator)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};
        auto                              enumerator = double_array_.get_enumerator();
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values };
        auto                              enumerator = double_array_.get_enumerator();
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values2 };
        auto                              enumerator = double_array_.get_enumerator();
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values };
        auto                              enumerator = double_array_.get_enumerator();

        boost::ignore_unused(enumerator.next());

        auto enumerator2 = enumerator;

        boost::ignore_unused(enumerator.next());

        const auto o_element = enumerator2.next();

        BOOST_REQUIRE(o_element);
        BOOST_TEST(o_element->first == "UTIGOSI");
        BOOST_TEST(o_element->second == 24);
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
        auto                              enumerator = double_array_.get_enumerator();

        {
            const auto o_element = enumerator.next();

            BOOST_CHECK(!o_element);
        }
        {
            const auto o_element = enumerator.next();

            BOOST_CHECK(!o_element);
        }
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values };
        auto                              enumerator = double_array_.get_enumerator();

        {
            const auto o_element = enumerator.next();

            BOOST_REQUIRE(o_element);
            BOOST_TEST(o_element->first == "SETA");
            BOOST_TEST(o_element->second == 42);
        }
        {
            const auto o_element = enumerator.next();

            BOOST_REQUIRE(o_element);
            BOOST_TEST(o_element->first == "UTIGOSI");
            BOOST_TEST(o_element->second == 24);
        }
        {
            const auto o_element = enumerator.next();

            BOOST_REQUIRE(o_element);
            BOOST_TEST(o_element->first == "UTO");
            BOOST_TEST(o_element->second == 2424);
        }
        {
            const auto o_element = enumerator.next();

            BOOST_CHECK(!o_element);
        }
        {
            const auto o_element = enumerator.next();

            BOOST_CHECK(!o_element);
        }
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values2 };
        auto                              enumerator = double_array_.get_enumerator();

        {
            const auto o_element = enumerator.next();

            BOOST_REQUIRE(o_element);
            const std::string expected_key{
                to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE6), to_c(0xB0), to_c(0xB4)
            }; // "Akamizu" in Kanji
            BOOST_TEST(o_element->first == expected_key);
            BOOST_TEST(o_element->second == 42);
        }
        {
            const auto o_element = enumerator.next();

            BOOST_REQUIRE(o_element);
            const std::string expected_key{
                to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE7), to_c(0x80), to_c(0xAC)
            }; // "Akase" in Kanji
            BOOST_TEST(o_element->first == expected_key);
            BOOST_TEST(o_element->second == 24);
        }
        {
            const auto o_element = enumerator.next();

            BOOST_CHECK(!o_element);
        }
        {
            const auto o_element = enumerator.next();

            BOOST_CHECK(!o_element);
        }
    }
    {
        // TODO: C style API
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
