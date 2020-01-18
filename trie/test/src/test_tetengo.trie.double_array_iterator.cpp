/*!
    \brief A double array iterator.

    Copyright (C) 2019 kaoru
 */

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/double_array_iterator.hpp>


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
BOOST_AUTO_TEST_SUITE(double_array_iterator)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array_iterator iterator{};
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{};
        auto                              iterator = std::begin(double_array_);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values };
        auto                              iterator = std::begin(double_array_);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values2 };
        auto                              iterator = std::begin(double_array_);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values };
        auto                              iterator = std::begin(double_array_);

        ++iterator;

        auto iterator2 = iterator;

        ++iterator;

        BOOST_TEST(iterator2->first == "UTIGOSI");
        BOOST_TEST(iterator2->second == 24);
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(operator_dereference)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array_iterator iterator{};

        BOOST_CHECK_THROW(*iterator, std::logic_error);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values };
        const auto                        iterator = std::begin(double_array_);

        BOOST_TEST(iterator->first == "SETA");
        BOOST_TEST(iterator->second == 42);
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array_iterator iterator1{};
        const tetengo::trie::double_array_iterator iterator2{};

        BOOST_CHECK(iterator1 == iterator2);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values };
        auto                              iterator1 = std::begin(double_array_);
        auto                              iterator2 = std::begin(double_array_);

        BOOST_CHECK(iterator1 == iterator2);

        ++iterator1;

        BOOST_CHECK(iterator1 != iterator2);

        ++iterator2;

        BOOST_CHECK(iterator1 == iterator2);

        ++iterator1;
        ++iterator1;
        ++iterator1;

        const tetengo::trie::double_array_iterator iterator3{};

        BOOST_CHECK(iterator1 == iterator3);
        BOOST_CHECK(iterator2 != iterator3);
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
        auto                              iterator = std::begin(double_array_);

        ++iterator;

        BOOST_CHECK(iterator == std::end(double_array_));
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values };
        auto                              iterator = std::begin(double_array_);

        BOOST_TEST(iterator->first == "SETA");
        BOOST_TEST(iterator->second == 42);

        ++iterator;

        BOOST_TEST(iterator->first == "UTIGOSI");
        BOOST_TEST(iterator->second == 24);

        ++iterator;

        BOOST_TEST(iterator->first == "UTO");
        BOOST_TEST(iterator->second == 2424);

        ++iterator;

        BOOST_CHECK(iterator == std::end(double_array_));
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_base_values2 };
        auto                              iterator = std::begin(double_array_);

        {
            const std::string expected_key{
                to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE6), to_c(0xB0), to_c(0xB4)
            }; // "Akamizu" in Kanji
            BOOST_TEST(iterator->first == expected_key);
            BOOST_TEST(iterator->second == 42);
        }
        {
            ++iterator;

            const std::string expected_key{
                to_c(0xE8), to_c(0xB5), to_c(0xA4), to_c(0xE7), to_c(0x80), to_c(0xAC)
            }; // "Akase" in Kanji
            BOOST_TEST(iterator->first == expected_key);
            BOOST_TEST(iterator->second == 24);
        }
        {
            ++iterator;

            BOOST_CHECK(iterator == std::end(double_array_));
        }
    }
    {
        // TODO: C style API
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
