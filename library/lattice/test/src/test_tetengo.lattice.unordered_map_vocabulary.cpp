/*! \file
    \brief An unordered_map vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>


namespace
{
    constexpr char to_c(unsigned char uc)
    {
        return uc;
    }

    const std::string key_mizuho{ to_c(0xE3), to_c(0x81), to_c(0xBF), to_c(0xE3), to_c(0x81),
                                  to_c(0x9A), to_c(0xE3), to_c(0x81), to_c(0xBB) };

    const std::string surface_mizuho{ to_c(0xE7), to_c(0x91), to_c(0x9E), to_c(0xE7), to_c(0xA9), to_c(0x82) };

    const std::string key_sakura{ to_c(0xE3), to_c(0x81), to_c(0x95), to_c(0xE3), to_c(0x81),
                                  to_c(0x8F), to_c(0xE3), to_c(0x82), to_c(0x89) };

    const std::string surface_sakura1{ to_c(0xE6), to_c(0xA1), to_c(0x9C) };

    const std::string surface_sakura2{ to_c(0xE3), to_c(0x81), to_c(0x95), to_c(0xE3), to_c(0x81),
                                       to_c(0x8F), to_c(0xE3), to_c(0x82), to_c(0x89) };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(unordered_map_vocabulary)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::unordered_map<std::string_view, std::vector<tetengo::lattice::entry>> map{};
        const tetengo::lattice::unordered_map_vocabulary                                 vocabulary{ map };
    }
    {
        const std::unordered_map<std::string_view, std::vector<tetengo::lattice::entry>> map{
            { key_mizuho, { { key_mizuho, surface_mizuho, 42 } } },
            { key_sakura, { { key_sakura, surface_sakura1, 24 }, { key_sakura, surface_sakura2, 2424 } } }
        };
        const tetengo::lattice::unordered_map_vocabulary vocabulary{ map };
    }
}

BOOST_AUTO_TEST_CASE(find)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::unordered_map<std::string_view, std::vector<tetengo::lattice::entry>> map{};
        const tetengo::lattice::unordered_map_vocabulary                                 vocabulary{ map };

        {
            const auto found = vocabulary.find(key_mizuho);
            BOOST_TEST(found.empty());
        }
        {
            const auto found = vocabulary.find(key_sakura);
            BOOST_TEST(found.empty());
        }
    }
    {
        const std::unordered_map<std::string_view, std::vector<tetengo::lattice::entry>> map{
            { key_mizuho, { { key_mizuho, surface_mizuho, 42 } } },
            { key_sakura, { { key_sakura, surface_sakura1, 24 }, { key_sakura, surface_sakura2, 2424 } } }
        };
        const tetengo::lattice::unordered_map_vocabulary vocabulary{ map };

        {
            const auto found = vocabulary.find(key_mizuho);
            BOOST_TEST_REQUIRE(found.size() == 1U);
            BOOST_TEST(found[0].key() == key_mizuho);
            BOOST_TEST(found[0].surface() == surface_mizuho);
            BOOST_TEST(found[0].cost() == 42);
        }
        {
            const auto found = vocabulary.find(key_sakura);
            BOOST_TEST_REQUIRE(found.size() == 2U);
            BOOST_TEST(found[0].key() == key_sakura);
            BOOST_TEST(found[0].surface() == surface_sakura1);
            BOOST_TEST(found[0].cost() == 24);
            BOOST_TEST(found[1].key() == key_sakura);
            BOOST_TEST(found[1].surface() == surface_sakura2);
            BOOST_TEST(found[1].cost() == 2424);
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
