/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

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

    const std::string key_sakura{ to_c(0xE3), to_c(0x81), to_c(0x95), to_c(0xE3), to_c(0x81),
                                  to_c(0x8F), to_c(0xE3), to_c(0x82), to_c(0x89) };

    class concrete_vocabulary : public tetengo::lattice::vocabulary
    {
    public:
        // constructors and destructors

        concrete_vocabulary(){};

        virtual ~concrete_vocabulary() = default;


    private:
        // virtual functions

        std::vector<tetengo::lattice::entry> find_impl(const std::string_view& key) const
        {
            if (key == key_mizuho)
            {
                return std::vector<tetengo::lattice::entry>{ tetengo::lattice::entry{
                    key_mizuho, surface_mizuho, 42 } };
            }
            else
            {
                return std::vector<tetengo::lattice::entry>{};
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

BOOST_AUTO_TEST_CASE(find)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_vocabulary vocabulary{};

        {
            const auto entries = vocabulary.find(key_mizuho);

            BOOST_TEST_REQUIRE(!entries.empty());
            BOOST_TEST(entries[0].key() == key_mizuho);
            BOOST_TEST(entries[0].surface() == surface_mizuho);
            BOOST_TEST(entries[0].cost() == 42);
        }
        {
            const auto entries = vocabulary.find(key_sakura);

            BOOST_TEST(entries.empty());
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
