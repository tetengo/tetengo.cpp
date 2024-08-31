/*! \file
    \brief The usage of trie search.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#include "usage_tetengo.trie.search_c.h"

// [search]
#include <assert.h>
#include <string.h>

#include <tetengo/trie/trie.h>
#include <tetengo/trie/trieIterator.h>


static void adding_observer(const char* const serialized_key, void* const p_context)
{
    strcat((char*)p_context, serialized_key);
    strcat((char*)p_context, "\n");
}

static void done_observer(void* const p_context)
{
    strcat((char*)p_context, "DONE");
}

void usage_tetengo_trie_search()
{
    // Prepares initial elements.
    const int initial_values[] = {
        -5, -3, 0, 5, 10, 14, 18, 22,
    };
    tetengo_trie_trieElement_t initial_elements[] = {
        // clang-format off
        { "tasakibashi", NULL },
        { "nihongiguchi", NULL },
        { "kumamotoekimae", NULL },
        { "gionbashi", NULL },
        { "gofukumachi", NULL },
        { "kawaramachi", NULL },
        { "keitokukoumae", NULL },
        { "karashimachou", NULL },
        // clang-format on
    };
    for (size_t i = 0; i < sizeof(initial_elements) / sizeof(tetengo_trie_trieElement_t); ++i)
    {
        initial_elements[i].p_value = &initial_values[i];
    }

    // Builds a trie with the initial elements.
    char                             building_observer_reports[128] = { '\0' };
    const tetengo_trie_trie_t* const p_trie = tetengo_trie_trie_create(
        initial_elements,
        sizeof(initial_elements) / sizeof(tetengo_trie_trieElement_t),
        sizeof(int),
        adding_observer,
        building_observer_reports,
        done_observer,
        building_observer_reports,
        tetengo_trie_trie_defaultDoubleArrayDensityFactor());
    assert(
        strcmp(
            building_observer_reports,
            "gionbashi\n"
            "gofukumachi\n"
            "karashimachou\n"
            "kawaramachi\n"
            "keitokukoumae\n"
            "kumamotoekimae\n"
            "nihongiguchi\n"
            "tasakibashi\n"
            "DONE") == 0);

    // Searches the trie.
    {
        // If a perfect-matching key is found, its value is returned.
        const int* const p_found_for_gionbashi = (const int*)tetengo_trie_trie_find(p_trie, "gionbashi");
        (void)p_found_for_gionbashi;
        assert(p_found_for_gionbashi);
        assert(*p_found_for_gionbashi == 5);
    }
    {
        // If not found, NULL is returned.
        const int* const p_found_for_hanabatachou = (const int*)tetengo_trie_trie_find(p_trie, "hanabatachou");
        (void)p_found_for_hanabatachou;
        assert(!p_found_for_hanabatachou);
    }

    // Creates a subtrie consisting of the elements with the common key prefix.
    const tetengo_trie_trie_t* const p_subtrie = tetengo_trie_trie_subtrie(p_trie, "ka");

    // Enumerates the values in the subtrie.
    int    subtrie_values[sizeof(initial_elements) / sizeof(tetengo_trie_trieElement_t)] = { 0 };
    size_t subtrie_value_count = 0;
    tetengo_trie_trieIterator_t* const p_iterator = tetengo_trie_trie_createIterator(p_subtrie);
    while (tetengo_trie_trieIterator_hasNext(p_iterator))
    {
        const int* const p_value = (const int*)tetengo_trie_trieIterator_get(p_iterator);
        subtrie_values[subtrie_value_count] = *p_value;
        ++subtrie_value_count;
        tetengo_trie_trieIterator_next(p_iterator);
    }
    tetengo_trie_trieIterator_destroy(p_iterator);
    assert(subtrie_value_count == 2U);
    assert(subtrie_values[0] == 22); // karashimachou
    assert(subtrie_values[1] == 14); // kawaramachi

    // Destroys the subtrie.
    tetengo_trie_trie_destroy(p_subtrie);

    // Destroys the trie.
    tetengo_trie_trie_destroy(p_trie);
}
// [search]
