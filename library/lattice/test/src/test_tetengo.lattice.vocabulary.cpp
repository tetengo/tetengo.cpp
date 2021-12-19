/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cstddef>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/connection.hpp>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/input.hpp>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/string_input.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace
{
    using key_type = tetengo::lattice::string_input;

    constexpr char operator""_c(const unsigned long long int uc)
    {
        return static_cast<char>(uc);
    }

    const std::string key_mizuho{ 0xE3_c, 0x81_c, 0xBF_c, 0xE3_c, 0x81_c, 0x9A_c, 0xE3_c, 0x81_c, 0xBB_c };

    const std::string surface_mizuho{ 0xE7_c, 0x91_c, 0x9E_c, 0xE7_c, 0xA9_c, 0x82_c };

    const std::any value_mizuho{ surface_mizuho };

    const std::string key_sakura{ 0xE3_c, 0x81_c, 0x95_c, 0xE3_c, 0x81_c, 0x8F_c, 0xE3_c, 0x82_c, 0x89_c };

    const std::string key_tsubame{ 0xE3_c, 0x81_c, 0xA4_c, 0xE3_c, 0x81_c, 0xB0_c, 0xE3_c, 0x82_c, 0x81_c };

    const std::string surface_tsubame{ 0xE7_c, 0x87_c, 0x95_c };

    const std::any value_tsubame{ surface_tsubame };

    class concrete_vocabulary : public tetengo::lattice::vocabulary
    {
    public:
        // constructors and destructors

        concrete_vocabulary(){};

        virtual ~concrete_vocabulary() = default;


    private:
        // virtual functions

        virtual std::vector<tetengo::lattice::entry_view>
        find_entries_impl(const tetengo::lattice::input& key) const override
        {
            if (key.as<key_type>().value() == key_mizuho)
            {
                static const tetengo::lattice::string_input key_input{ key_mizuho };
                return std::vector<tetengo::lattice::entry_view>{ tetengo::lattice::entry_view{
                    &key_input, &value_mizuho, 42 } };
            }
            else if (key.as<key_type>().value() == key_tsubame)
            {
                static const tetengo::lattice::string_input key_input{ key_tsubame };
                return std::vector<tetengo::lattice::entry_view>{ tetengo::lattice::entry_view{
                    &key_input, &value_tsubame, 24 } };
            }
            else
            {
                return std::vector<tetengo::lattice::entry_view>{};
            }
        }

        virtual tetengo::lattice::connection
        find_connection_impl(const tetengo::lattice::node& from, const tetengo::lattice::entry_view& to) const override
        {
            if (from.p_key() && from.p_key()->is<tetengo::lattice::string_input>() &&
                from.p_key()->as<tetengo::lattice::string_input>().value() == key_mizuho && to.p_key() &&
                to.p_key()->is<tetengo::lattice::string_input>() &&
                to.p_key()->as<tetengo::lattice::string_input>().value() == key_mizuho)
            {
                return tetengo::lattice::connection{ 42 };
            }
            else
            {
                return tetengo::lattice::connection{ std::numeric_limits<int>::max() };
            }
        }
    };

    tetengo::lattice::node make_node(const tetengo::lattice::entry_view& entry)
    {
        static const std::vector<int> preceding_edge_costs{};
        return tetengo::lattice::node{ entry,
                                       std::numeric_limits<std::size_t>::max(),
                                       &preceding_edge_costs,
                                       std::numeric_limits<std::size_t>::max(),
                                       std::numeric_limits<int>::max() };
    }


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
            const auto entries = vocabulary.find_entries(key_type{ key_mizuho });

            BOOST_TEST_REQUIRE(!std::empty(entries));
            BOOST_TEST_REQUIRE(entries[0].p_key());
            BOOST_TEST_REQUIRE(entries[0].p_key()->is<tetengo::lattice::string_input>());
            BOOST_TEST(entries[0].p_key()->as<tetengo::lattice::string_input>().value() == key_mizuho);
            BOOST_TEST(*std::any_cast<std::string>(entries[0].value()) == surface_mizuho);
            BOOST_TEST(entries[0].cost() == 42);
        }
        {
            const auto entries = vocabulary.find_entries(key_type{ key_sakura });

            BOOST_TEST(std::empty(entries));
        }
    }
}

BOOST_AUTO_TEST_CASE(find_connection)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_vocabulary vocabulary{};

        {
            const auto entries_mizuho = vocabulary.find_entries(key_type{ key_mizuho });
            BOOST_TEST_REQUIRE(std::size(entries_mizuho) == 1U);

            const auto connection = vocabulary.find_connection(make_node(entries_mizuho[0]), entries_mizuho[0]);

            BOOST_TEST(connection.cost() == 42);
        }
        {
            const auto entries_mizuho = vocabulary.find_entries(key_type{ key_mizuho });
            BOOST_TEST_REQUIRE(std::size(entries_mizuho) == 1U);
            const auto entries_tsubame = vocabulary.find_entries(key_type{ key_tsubame });
            BOOST_TEST_REQUIRE(std::size(entries_tsubame) == 1U);

            const auto connection = vocabulary.find_connection(make_node(entries_mizuho[0]), entries_tsubame[0]);

            BOOST_TEST(connection.cost() == std::numeric_limits<int>::max());
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
