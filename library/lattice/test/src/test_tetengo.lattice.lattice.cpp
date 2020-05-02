/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace
{
    std::unique_ptr<tetengo::lattice::vocabulary> create_vocabulary()
    {
        /*
                  +------------------mizuho/sakura/tsubame-------------------+
                  |                                                          |
                  +------------ariake/rapid811------------+                  |
                  |                                       |                  |
            (Hakata)--kamome/local415--(Tosu)--local813--(Omuta)--local817--(Kumamoto)
                                           |                                 |
                                           +------------local815-------------+
        */
        std::vector<std::pair<std::string, std::vector<tetengo::lattice::entry>>> entries{
            { "[HakataTosu][TosuOmuta][OmutaKumamoto]",
              {
                  { "Hakata-Tosu-Omuta-Kumamoto", std::string{ "mizuho" }, 3670 },
                  { "Hakata-Tosu-Omuta-Kumamoto", std::string{ "sakura" }, 2620 },
                  { "Hakata-Tosu-Omuta-Kumamoto", std::string{ "tsubame" }, 2390 },
              } },
            { "[HakataTosu][TosuOmuta]",
              {
                  { "Hakata-Tosu-Omuta", std::string{ "ariake" }, 2150 },
                  { "Hakata-Tosu-Omuta", std::string{ "rapid811" }, 1310 },
              } },
            { "[HakataTosu]",
              {
                  { "Hakata-Tosu", std::string{ "kamome" }, 840 },
                  { "Hakata-Tosu", std::string{ "local415" }, 570 },
              } },
            { "[TosuOmuta]",
              {
                  { "Tosu-Omuta", std::string{ "local813" }, 860 },
              } },
            { "[TosuOmuta][OmutaKumamoto]",
              {
                  { "Tosu-Omuta-Kumamoto", std::string{ "local815" }, 1680 },
              } },
            { "[OmutaKumamoto]",
              {
                  { "Omuta-Kumamoto", std::string{ "local817" }, 950 },
              } },
        };
        std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>> connections{};
        return std::make_unique<tetengo::lattice::unordered_map_vocabulary>(std::move(entries), std::move(connections));
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(lattice)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::lattice lattice_{ create_vocabulary() };
    }
}

BOOST_AUTO_TEST_CASE(push_back)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::lattice::lattice lattice_{ create_vocabulary() };

        lattice_.push_back("[HakataTosu]");
        lattice_.push_back("[TosuOmuta]");
        lattice_.push_back("[OmutaKumamoto]");
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
