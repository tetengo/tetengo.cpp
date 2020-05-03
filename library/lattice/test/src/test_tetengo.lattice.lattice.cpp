/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace
{
    std::unique_ptr<tetengo::lattice::vocabulary> create_vocabulary()
    {
        static const tetengo::lattice::entry bos_eos_entry{ std::string{ tetengo::lattice::node::bos().key() },
                                                            tetengo::lattice::node::bos().value(),
                                                            tetengo::lattice::node::bos().node_cost() };

        /*
                       +------------------mizuho/sakura/tsubame-------------------+
                       |                path cost: 4270/3220/2990                 |
                       |                                                          |
                       +------------ariake/rapid811------------+                  |
                       |          path cost: 2850/2010         |                  |
                       |                                       |                  |
            BOS--(Hakata)--kamome/local415--(Tosu)--local813--(Omuta)--local817--(Kumamoto)--EOS
                         path cost: 1640/1370   |   pc: 2830           pc: 3160   |     path cost:3390
                                                |                                 |
                                                +------------local815-------------+
                                                          path cost: 3550
            (1) 3390  BOS - tsubame - EOS
            (2) 3620  BOS - sakura - EOS
            (3) 3760  BOS - rapid811 - local817 - EOS
            (4) 4050  BOS - local415 - local815 - EOS
            (5) 4320  BOS - kamome - local815 - EOS
            (6) 4600  BOS - ariake - local817 - EOS
            (7) 4670  BOS - mizuho - EOS
            (8) 4680  BOS - local415 - local813 - local817 - EOS
            (9) 4950  BOS - kamome - local813 - local817 - EOS
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
        std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>> connections{
            { { bos_eos_entry, { "Hakata-Tosu-Omuta-Kumamoto", {}, 0 } }, 600 },
            { { bos_eos_entry, { "Hakata-Tosu-Omuta", {}, 0 } }, 700 },
            { { bos_eos_entry, { "Hakata-Tosu", {}, 0 } }, 800 },
            { { { "Hakata-Tosu", {}, 0 }, { "Tosu-Omuta-Kumamoto", {}, 0 } }, 500 },
            { { { "Hakata-Tosu", {}, 0 }, { "Tosu-Omuta", {}, 0 } }, 600 },
            { { { "Hakata-Tosu-Omuta", {}, 0 }, { "Omuta-Kumamoto", {}, 0 } }, 200 },
            { { { "Tosu-Omuta", {}, 0 }, { "Omuta-Kumamoto", {}, 0 } }, 300 },
            { { { "Hakata-Tosu-Omuta-Kumamoto", {}, 0 }, bos_eos_entry }, 400 },
            { { { "Tosu-Omuta-Kumamoto", {}, 0 }, bos_eos_entry }, 500 },
            { { { "Omuta-Kumamoto", {}, 0 }, bos_eos_entry }, 600 },
        };
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
