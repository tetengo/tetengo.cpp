/*! \file
    \brief The usage of Viterbi search.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include "usage_tetengo.lattice.viterbi_c.h"

/* [viterbi] */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tetengo/lattice/connection.h>
#include <tetengo/lattice/constraint.h>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/input.h>
#include <tetengo/lattice/lattice.h>
#include <tetengo/lattice/nBestIterator.h>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/path.h>
#include <tetengo/lattice/stringView.h>
#include <tetengo/lattice/vocabulary.h>


tetengo_lattice_vocabulary_t* build_vocabulary();
const char*                   to_string(const tetengo_lattice_path_t* p_path);

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

    /* Builds a vocabulary. */
    const tetengo_lattice_vocabulary_t* const p_vocabulary = build_vocabulary();

    /* Creates an object for a lattice. */
    tetengo_lattice_lattice_t* const p_lattice = tetengo_lattice_lattice_create(p_vocabulary);

    /* Enters key characters to construct the lattice. */
    tetengo_lattice_input_t* const p_input_a = tetengo_lattice_input_createStringInput("a");
    tetengo_lattice_input_t* const p_input_b = tetengo_lattice_input_createStringInput("b");
    tetengo_lattice_lattice_pushBack(p_lattice, p_input_a);
    tetengo_lattice_lattice_pushBack(p_lattice, p_input_b);
    {
        /* Finishes the lattice construction. */
        const size_t eos_preceding_count = tetengo_lattice_lattice_settle(p_lattice, NULL, NULL);
        if (eos_preceding_count == 0)
        {
            return;
        }
        {
            tetengo_lattice_node_t eos_node;
            int* const             p_eos_preceding_costs = (int*)malloc(eos_preceding_count * sizeof(int));
            tetengo_lattice_lattice_settle(p_lattice, &eos_node, p_eos_preceding_costs);
            {
                /* Creates an iterator to enumerate the paths in the lattice. */
                tetengo_lattice_nBestIterator_t* const p_iterator = tetengo_lattice_nBestIterator_create(
                    p_lattice, &eos_node, tetengo_lattice_constraint_createEmpty());

                /* Enumerates the paths. */
                char paths[256] = { 0 };
                while (tetengo_lattice_nBestIterator_hasNext(p_iterator))
                {
                    /* Obtains the current path. */
                    tetengo_lattice_path_t* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);

                    strcat(paths, to_string(p_path));
                    tetengo_lattice_path_destroy(p_path);

                    /* Moves to the next path. */
                    tetengo_lattice_nBestIterator_next(p_iterator);
                }
                {
                    /* clang-format off */
                    static const char* const expected =
                        "[BOS]-[Alice]-[Bravo]-[EOS] (12)\n"
                        "[BOS]-[AwaBizan]-[EOS] (17)\n"
                        "[BOS]-[Alpha]-[Bravo]-[EOS] (18)\n"
                        "[BOS]-[Alpha]-[Bob]-[EOS] (24)\n"
                        "[BOS]-[Alice]-[Bob]-[EOS] (25)\n";
                    /* clang-format on */
                    assert(strcmp(paths, expected) == 0);
                }
                tetengo_lattice_nBestIterator_destroy(p_iterator);
            }
            free((void*)p_eos_preceding_costs);
        }
    }
    tetengo_lattice_lattice_destroy(p_lattice);
    tetengo_lattice_vocabulary_destroy(p_vocabulary);
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
    /* The contents of the vocabulary. */
    const tetengo_lattice_entry_t entries[] = {
        /* clang-format off */
        { { "a", 1 }, "Alpha", 2 },
        { { "a", 1 }, "Alice", 1 },
        { { "b", 1 }, "Bravo", 7 },
        { { "b", 1 }, "Bob", 8 },
        { { "ab", 2 }, "AwaBizan", 9 },
        /* clang-format on */
    };
    const tetengo_lattice_entry_t* p_entries[5] = { NULL };
    p_entries[0] = &entries[0];
    p_entries[1] = &entries[1];
    p_entries[2] = &entries[2];
    p_entries[3] = &entries[3];
    p_entries[4] = &entries[4];
    {
        tetengo_lattice_entry_t bosEos;
        bosEos.key = tetengo_lattice_entry_bosEos()->key;
        bosEos.p_value = tetengo_lattice_entry_valueOf(tetengo_lattice_entry_bosEos()->value_handle);
        bosEos.cost = tetengo_lattice_entry_bosEos()->cost;
        {
            tetengo_lattice_keyEntriesPair_t entry_mappings[3];
            entry_mappings[0].key.p_head = "a";
            entry_mappings[0].key.length = 1;
            entry_mappings[0].p_entries = &p_entries[0];
            entry_mappings[0].entry_count = 2;
            entry_mappings[1].key.p_head = "b";
            entry_mappings[1].key.length = 1;
            entry_mappings[1].p_entries = &p_entries[2];
            entry_mappings[1].entry_count = 2;
            entry_mappings[2].key.p_head = "ab";
            entry_mappings[2].key.length = 2;
            entry_mappings[2].p_entries = &p_entries[4];
            entry_mappings[2].entry_count = 1;
            {
                tetengo_lattice_entriesConnectionCostPair_t connections[10];
                connections[0].p_from = &bosEos;
                connections[0].p_to = &entries[0];
                connections[0].cost = 3;
                connections[1].p_from = &bosEos;
                connections[1].p_to = &entries[1];
                connections[1].cost = 1;
                connections[2].p_from = &entries[0];
                connections[2].p_to = &entries[2];
                connections[2].cost = 4;
                connections[3].p_from = &entries[1];
                connections[3].p_to = &entries[2];
                connections[3].cost = 1;
                connections[4].p_from = &entries[0];
                connections[4].p_to = &entries[3];
                connections[4].cost = 5;
                connections[5].p_from = &entries[1];
                connections[5].p_to = &entries[3];
                connections[5].cost = 9;
                connections[6].p_from = &entries[2];
                connections[6].p_to = &bosEos;
                connections[6].cost = 2;
                connections[7].p_from = &entries[3];
                connections[7].p_to = &bosEos;
                connections[7].cost = 6;
                connections[8].p_from = &bosEos;
                connections[8].p_to = &entries[4];
                connections[8].cost = 7;
                connections[9].p_from = &entries[4];
                connections[9].p_to = &bosEos;
                connections[9].cost = 1;

                /* Returns a vocabulary implemented with hash tables. */
                return tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
                    entry_mappings,
                    sizeof(entry_mappings) / sizeof(tetengo_lattice_keyEntriesPair_t),
                    connections,
                    sizeof(connections) / sizeof(tetengo_lattice_entriesConnectionCostPair_t),
                    entry_hash,
                    entry_equal_to);
            }
        }
    }
}

static const char* value_of(const tetengo_lattice_node_t* const p_node, const int first)
{
    /* The value is accessible by the handle. */
    const char* const value = (const char*)tetengo_lattice_entry_valueOf(p_node->value_handle);
    if (value)
    {
        return value;
    }
    else if (first)
    {
        return "BOS";
    }
    else
    {
        return "EOS";
    }
}

const char* to_string(const tetengo_lattice_path_t* const p_path)
{
    static char result[48] = { 0 };
    result[0] = '\0';
    {
        /* Each path object holds the nodes that make up itself, and the whole cost. */
        const size_t                  node_count = tetengo_lattice_path_pNodes(p_path, NULL);
        tetengo_lattice_node_t* const p_nodes =
            (tetengo_lattice_node_t*)malloc(node_count * sizeof(tetengo_lattice_node_t));
        if (!p_nodes)
        {
            return result;
        }
        tetengo_lattice_path_pNodes(p_path, p_nodes);
        {
            size_t i = 0;
            for (i = 0; i < node_count; ++i)
            {
                if (i > 0)
                {
                    strcat(result, "-");
                }
                strcat(result, "[");
                strcat(result, value_of(&p_nodes[i], i == 0));
                strcat(result, "]");
            }
            strcat(result, " (");
            sprintf(result + strlen(result), "%d", tetengo_lattice_path_cost(p_path));
            strcat(result, ")");
        }
        free(p_nodes);
    }
    strcat(result, "\n");
    return result;
}
/* [viterbi] */
