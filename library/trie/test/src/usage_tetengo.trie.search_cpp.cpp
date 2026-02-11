/*! \file
    \brief The usage of trie search.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include "usage_tetengo.trie.search_cpp.hpp"

// [search]
#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <boost/stl_interfaces/iterator_interface.hpp> // IWYU pragma: keep

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/trie_iterator.hpp>


namespace usage_tetengo::trie
{
    void search()
    {
        // Prepares a trie building observer set.
        // The observer set records the inserted keys and the end.
        std::vector<std::string>                                                building_observer_reports{};
        const tetengo::trie::trie<std::string, int>::building_observer_set_type building_observer_set{
            [&building_observer_reports](const std::string_view& key) {
                building_observer_reports.push_back(std::string{ key });
            },
            [&building_observer_reports]() { building_observer_reports.push_back("DONE"); }
        };

        // Builds a trie with initial elements.
        const tetengo::trie::trie<std::string, int> trie_{ {
                                                               { "tasakibashi", -5 },
                                                               { "nihongiguchi", -3 },
                                                               { "kumamotoekimae", 0 },
                                                               { "gionbashi", 5 },
                                                               { "gofukumachi", 10 },
                                                               { "kawaramachi", 14 },
                                                               { "keitokukoumae", 18 },
                                                               { "karashimachou", 22 },
                                                           },
                                                           tetengo::trie::default_serializer<std::string>{ true },
                                                           building_observer_set };
        assert(
            (building_observer_reports == std::vector<std::string>{
                                              "gionbashi",
                                              "gofukumachi",
                                              "karashimachou",
                                              "kawaramachi",
                                              "keitokukoumae",
                                              "kumamotoekimae",
                                              "nihongiguchi",
                                              "tasakibashi",
                                              "DONE",
                                          }));

        // Searches the trie.
        // If a perfect-matching key is found, its value is returned.
        [[maybe_unused]] const int* const p_found_for_gionbashi = trie_.find("gionbashi");
        assert(p_found_for_gionbashi);
        assert(*p_found_for_gionbashi == 5);

        // If not found, nullptr is returned.
        [[maybe_unused]] const int* const p_found_for_hanabatachou = trie_.find("hanabatachou");
        assert(!p_found_for_hanabatachou);

        // Creates a subtrie consisting of the elements with the common key prefix.
        const auto p_subtrie = trie_.subtrie("ka");

        // Enumerates the values in the subtrie.
        std::vector<int> subtrie_values{};
        std::copy(std::begin(*p_subtrie), std::end(*p_subtrie), std::back_inserter(subtrie_values));
        assert(
            (subtrie_values == std::vector<int>{
                                   22, // karashimachou
                                   14, // kawaramachi
                               }));
    }
}
// [search]
