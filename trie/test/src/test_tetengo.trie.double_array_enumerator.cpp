/*!
    \brief A double array enumerator.

    Copyright (C) 2019 kaoru
 */

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>
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
        const tetengo::trie::double_array_enumerator enumerator{};
    }
    {
        // TODO: C style API
    }

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

        ++enumerator;

        auto enumerator2 = enumerator;

        ++enumerator;

        BOOST_TEST(enumerator2->first == "UTIGOSI");
        BOOST_TEST(enumerator2->second == 24);
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(operator_dereference)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array_enumerator enumerator{};

        BOOST_CHECK_THROW(*enumerator, std::logic_error);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values };
        const auto                        enumerator = double_array_.get_enumerator();

        BOOST_TEST(enumerator->first == "SETA");
        BOOST_TEST(enumerator->second == 42);
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array_enumerator enumerator1{};
        const tetengo::trie::double_array_enumerator enumerator2{};

        BOOST_CHECK(enumerator1 == enumerator2);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values };
        auto                              enumerator1 = double_array_.get_enumerator();
        auto                              enumerator2 = double_array_.get_enumerator();

        BOOST_CHECK(enumerator1 == enumerator2);

        ++enumerator1;

        BOOST_CHECK(enumerator1 != enumerator2);

        ++enumerator2;

        BOOST_CHECK(enumerator1 == enumerator2);

        ++enumerator1;
        ++enumerator1;
        ++enumerator1;

        const tetengo::trie::double_array_enumerator enumerator3{};

        BOOST_CHECK(enumerator1 == enumerator3);
        BOOST_CHECK(enumerator2 != enumerator3);
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(increment)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};
        auto                              enumerator = double_array_.get_enumerator();

        ++enumerator;

        BOOST_CHECK(enumerator == tetengo::trie::double_array_enumerator{});
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values };
        auto                              enumerator = double_array_.get_enumerator();

        BOOST_TEST(enumerator->first == "SETA");
        BOOST_TEST(enumerator->second == 42);

        ++enumerator;

        BOOST_TEST(enumerator->first == "UTIGOSI");
        BOOST_TEST(enumerator->second == 24);

        ++enumerator;

        BOOST_TEST(enumerator->first == "UTO");
        BOOST_TEST(enumerator->second == 2424);

        ++enumerator;

        BOOST_CHECK(enumerator == tetengo::trie::double_array_enumerator{});
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values2 };
        auto                              enumerator = double_array_.get_enumerator();

        {
            const std::string expected_key{
                to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE6), to_c(0xB0), to_c(0xB4)
            }; // "Akamizu" in Kanji
            BOOST_TEST(enumerator->first == expected_key);
            BOOST_TEST(enumerator->second == 42);
        }
        {
            ++enumerator;

            const std::string expected_key{
                to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE7), to_c(0x80), to_c(0xAC)
            }; // "Akase" in Kanji
            BOOST_TEST(enumerator->first == expected_key);
            BOOST_TEST(enumerator->second == 24);
        }
        {
            ++enumerator;

            BOOST_CHECK(enumerator == tetengo::trie::double_array_enumerator{});
        }
    }
    {
        // TODO: C style API
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
