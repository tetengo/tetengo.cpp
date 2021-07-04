/*! \file
    \brief The usage of Viterbi search

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include "usage_tetengo.lattice.viterbi_c.h"

/* [viterbi] */
#include <string.h>

#include <tetengo/lattice/connection.h>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/lattice.h>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/vocabulary.h>


tetengo_lattice_vocabulary_t* build_vocabulary();

void usage_tetengo_lattice_viterbi()
{
    /*
        Makes the following lattice and searches it.

                /-----[ab:AwaBizan]-----\
               /  (7)      (9)      (1)  \
              /                           \
             /       (2)   (4)   (7)       \
        [BOS]-----[a:Alpha]---[b:Bravo]-----[EOS]
             \ (3)         \ /(1)      (2) /
              \(1)          X             /(6)
               \           / \(5)        /
                `-[a:Alice]---[b:Bob]---'
                     (1)   (9)  (8)
        Path                         Cost
        [BOS]-[Alice]-[Bravo]-[EOS]  1+1+1+7+2=12
        [BOS]---[AwaBizan]----[EOS]  7+9+1    =17
        [BOS]-[Alpha]-[Bravo]-[EOS]  3+2+4+7+2=18
        [BOS]-[Alpha]-[Bob]---[EOS]  3+2+5+8+6=24
        [BOS]-[Alice]-[Bob]---[EOS]  1+1+9+8+6=25
    */

    tetengo_lattice_vocabulary_t* const p_vocabulary = build_vocabulary();

    tetengo_lattice_lattice_t* const p_lattice = tetengo_lattice_lattice_create(p_vocabulary);

    tetengo_lattice_lattice_pushBack(p_lattice, "a");
    tetengo_lattice_lattice_pushBack(p_lattice, "b");

    tetengo_lattice_node_t eos_node = { 0 };
    {
        const size_t eos_preceding_count = tetengo_lattice_lattice_settle(p_lattice, NULL, NULL);
        if (eos_preceding_count == 0)
        {
            return;
        }
        int* const p_eos_preceding_costs = (int*)malloc(eos_preceding_count * sizeof(int));
        tetengo_lattice_lattice_settle(p_lattice, &eos_node, p_eos_preceding_costs);
        free((void*)p_eos_preceding_costs);
    }

    tetengo_lattice_lattice_destroy(p_lattice);
}

static size_t hash(const char* const string, const size_t length)
{
    size_t value = 0;
    size_t i = 0;
    for (i = 0; i < length; ++i)
    {
        value ^= string[i];
    }
    return value;
}

static size_t entry_hash(const tetengo_lattice_entryView_t* const p_entry)
{
    const size_t      key_hash = hash(p_entry->key.p_head, p_entry->key.length);
    const char* const value = (const char*)tetengo_lattice_entry_valueOf(p_entry->value_handle);
    const size_t      value_hash = value ? hash(value, strlen(value)) : 0;
    return key_hash ^ value_hash;
}

static int
entry_equal_to(const tetengo_lattice_entryView_t* const p_entry1, const tetengo_lattice_entryView_t* const p_entry2)
{
    const char* const value1 = (const char*)tetengo_lattice_entry_valueOf(p_entry1->value_handle);
    const char* const value2 = (const char*)tetengo_lattice_entry_valueOf(p_entry2->value_handle);
    return p_entry1->key.length == p_entry2->key.length &&
           strncmp(p_entry1->key.p_head, p_entry2->key.p_head, p_entry1->key.length) == 0 &&
           ((!value1 && !value2) || (value1 && value2 && strcmp(value1, value2) == 0));
}

tetengo_lattice_vocabulary_t* build_vocabulary()
{
    const tetengo_lattice_entry_t entries[] = {
        // clang-format off
        { { "a", 1 }, "Alpha", 2 },
        { { "a", 1 }, "Alice", 1 },
        { { "b", 1 }, "Bravo", 7 },
        { { "b", 1 }, "Bob", 8 },
        { { "ab", 2 }, "AwaBizan", 9 },
        // clang-format on
    };
    const tetengo_lattice_entry_t          bosEos = { tetengo_lattice_entry_bosEos()->key,
                                             tetengo_lattice_entry_valueOf(
                                                 tetengo_lattice_entry_bosEos()->value_handle),
                                             tetengo_lattice_entry_bosEos()->cost };
    const tetengo_lattice_keyEntriesPair_t entry_mappings[] = {
        // clang-format off
        { { "a", 1 }, &entries[0], 2 },
        { { "b", 1 }, &entries[2], 2 },
        { { "ab", 2 }, &entries[4], 1 },
        // clang-format on
    };
    const tetengo_lattice_entriesConnectionCostPair_t connections[] = {
        // clang-format off
        { &bosEos, &entries[0], 3 },
        { &bosEos, &entries[1], 1 },
        { &entries[0], &entries[2], 4 },
        { &entries[1], &entries[2], 1 },
        { &entries[0], &entries[3], 5 },
        { &entries[1], &entries[3], 9 },
        { &entries[2], &bosEos, 2 },
        { &entries[3], &bosEos, 6 },
        { &bosEos, &entries[4], 7 },
        { &entries[4], &bosEos, 1 },
        // clang-format on
    };
    return tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
        entry_mappings,
        sizeof(entry_mappings) / sizeof(tetengo_lattice_keyEntriesPair_t),
        connections,
        sizeof(connections) / sizeof(tetengo_lattice_entriesConnectionCostPair_t),
        entry_hash,
        entry_equal_to);
}
/* [viterbi] */
