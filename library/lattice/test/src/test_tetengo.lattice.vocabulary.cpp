/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/connection.hpp>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace
{
    constexpr char to_c(unsigned char uc)
    {
        return uc;
    }

    const std::string key_mizuho{ to_c(0xE3), to_c(0x81), to_c(0xBF), to_c(0xE3), to_c(0x81),
                                  to_c(0x9A), to_c(0xE3), to_c(0x81), to_c(0xBB) };

    const std::string surface_mizuho{ to_c(0xE7), to_c(0x91), to_c(0x9E), to_c(0xE7), to_c(0xA9), to_c(0x82) };

    const std::any value_mizuho{ surface_mizuho };

    const std::string key_sakura{ to_c(0xE3), to_c(0x81), to_c(0x95), to_c(0xE3), to_c(0x81),
                                  to_c(0x8F), to_c(0xE3), to_c(0x82), to_c(0x89) };

    const std::string key_tsubame{ to_c(0xE3), to_c(0x81), to_c(0xA4), to_c(0xE3), to_c(0x81),
                                   to_c(0xB0), to_c(0xE3), to_c(0x82), to_c(0x81) };

    const std::string surface_tsubame{ to_c(0xE7), to_c(0x87), to_c(0x95) };

    const std::any value_tsubame{ surface_tsubame };

    class concrete_vocabulary : public tetengo::lattice::vocabulary
    {
    public:
        // constructors and destructors

        concrete_vocabulary(){};

        virtual ~concrete_vocabulary() = default;


    private:
        // virtual functions

        virtual std::vector<tetengo::lattice::entry_view> find_entries_impl(const std::string_view& key) const override
        {
            if (key == key_mizuho)
            {
                return std::vector<tetengo::lattice::entry_view>{ tetengo::lattice::entry_view{
                    key_mizuho, &value_mizuho, 42 } };
            }
            else if (key == key_tsubame)
            {
                return std::vector<tetengo::lattice::entry_view>{ tetengo::lattice::entry_view{
                    key_tsubame, &value_tsubame, 24 } };
            }
            else
            {
                return std::vector<tetengo::lattice::entry_view>{};
            }
        }

        virtual tetengo::lattice::connection find_connection_impl(
            const tetengo::lattice::entry_view& from,
            const tetengo::lattice::entry_view& to) const override
        {
            if (from.key() == key_mizuho && to.key() == key_mizuho)
            {
                return tetengo::lattice::connection{ 42 };
            }
            else
            {
                return tetengo::lattice::connection{ std::numeric_limits<int>::max() };
            }
        }
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(vocabulary)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_vocabulary vocabulary{};
    }
}

BOOST_AUTO_TEST_CASE(find_entries)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_vocabulary vocabulary{};

        {
            const auto entries = vocabulary.find_entries(key_mizuho);

            BOOST_TEST_REQUIRE(!entries.empty());
            BOOST_TEST(entries[0].key() == key_mizuho);
            BOOST_TEST(*std::any_cast<std::string>(entries[0].value()) == surface_mizuho);
            BOOST_TEST(entries[0].cost() == 42);
        }
        {
            const auto entries = vocabulary.find_entries(key_sakura);

            BOOST_TEST(entries.empty());
        }
    }
}

BOOST_AUTO_TEST_CASE(find_connection)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_vocabulary vocabulary{};

        {
            const auto entries_mizuho = vocabulary.find_entries(key_mizuho);
            BOOST_TEST_REQUIRE(entries_mizuho.size() == 1U);

            const auto connection = vocabulary.find_connection(entries_mizuho[0], entries_mizuho[0]);

            BOOST_TEST(connection.cost() == 42);
        }
        {
            const auto entries_mizuho = vocabulary.find_entries(key_mizuho);
            BOOST_TEST_REQUIRE(entries_mizuho.size() == 1U);
            const auto entries_tsubame = vocabulary.find_entries(key_tsubame);
            BOOST_TEST_REQUIRE(entries_tsubame.size() == 1U);

            const auto connection = vocabulary.find_connection(entries_mizuho[0], entries_tsubame[0]);

            BOOST_TEST(connection.cost() == std::numeric_limits<int>::max());
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
