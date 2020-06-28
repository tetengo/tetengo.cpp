/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_LATTICE_VOCABULARY_H)
#define TETENGO_LATTICE_VOCABULARY_H

#include <stddef.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_vocabulary_tag                tetengo_lattice_vocabulary_t;
typedef struct tetengo_lattice_entryView_tag                 tetengo_lattice_entryView_t;
typedef struct tetengo_lattice_node_tag                      tetengo_lattice_node_t;
typedef struct tetengo_lattice_keyEntriesPair_tag            tetengo_lattice_keyEntriesPair_t;
typedef struct tetengo_lattice_connection_tag                tetengo_lattice_connection_t;
typedef struct tetengo_lattice_entriesConnectionCostPair_tag tetengo_lattice_entriesConnectionCostPair_t;
#endif


/*!
    \brief Creates an unordered_map vocabulary.

    \param p_entries        A pointer to entries.
    \param entry_count      An entry count.
    \param p_connections    A pointer to connections.
    \param connection_count A connection count.
    \param p_entry_hash     A pointer to a hash function for an entry.

    \return A pointer to an unordered_map vocabulary. Or NULL when p_entries and/or p_connections are NULL.
*/
tetengo_lattice_vocabulary_t* tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
    const tetengo_lattice_keyEntriesPair_t*            p_entries,
    size_t                                             entry_count,
    const tetengo_lattice_entriesConnectionCostPair_t* p_connections,
    size_t                                             connection_count,
    size_t (*p_entry_hash)(const tetengo_lattice_entryView_t*));

/*!
    \brief Destroys a vocabulary.

    \param p_vocabulary A pointer to a vocabulary.
*/
void tetengo_lattice_vocabulary_destroy(const tetengo_lattice_vocabulary_t* p_vocabulary);

/*!
    \brief Finds entries.

    \param p_vocabulary A pointer to a vocabulary.
    \param key          A key.
    \param p_entries    The storage for output entries. Can be NULL.

    \return An entry count.
*/
size_t tetengo_lattice_vocabulary_findEntries(
    const tetengo_lattice_vocabulary_t* p_vocabulary,
    const char*                         key,
    tetengo_lattice_entryView_t*        p_entries);

/*!
    \brief Finds a connection between entries.

    \param p_vocabulary A pointer to a vocabulary.
    \param p_from       A pointer to an origin node.
    \param p_to         A pointer to a destination entry.
    \param p_connection The storage for an output connection.

    \retval non-zero When an output connection is stored.
    \retval 0        Otherwise.
*/
int tetengo_lattice_vocabulary_findConnection(
    const tetengo_lattice_vocabulary_t* p_vocabulary,
    const tetengo_lattice_node_t*       p_from,
    const tetengo_lattice_entryView_t*  p_to,
    tetengo_lattice_connection_t*       p_connection);


#if defined(__cplusplus)
}
#endif


#endif
