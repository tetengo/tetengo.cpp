/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <string>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/entry.hpp>


namespace
{
    constexpr char to_c(unsigned char uc)
    {
        return uc;
    }

    const std::string key_mizuho{ to_c(0xE3), to_c(0x81), to_c(0xBF), to_c(0xE3), to_c(0x81),
                                  to_c(0x9A), to_c(0xE3), to_c(0x81), to_c(0xBB) };

    const std::string surface_mizuho{ to_c(0xE7), to_c(0x91), to_c(0x9E), to_c(0xE7), to_c(0xA9), to_c(0x82) };

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(entry)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::entry entry_{ key_mizuho, surface_mizuho, 42 };
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
