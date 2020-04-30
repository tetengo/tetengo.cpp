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
typedef struct tetengo_lattice_vocabulary_tag             tetengo_lattice_vocabulary;
typedef struct tetengo_lattice_entry_tag                  tetengo_lattice_entry;
typedef struct tetengo_lattice_entry_map_element_tag      tetengo_lattice_entry_map_element;
typedef struct tetengo_lattice_connection_tag             tetengo_lattice_connection;
typedef struct tetengo_lattice_connection_map_element_tag tetengo_lattice_connection_map_element;
#endif


/*!
    \brief Creates an unordered_map vocabulary.

    \param p_entry_map         An entry map.
    \param entry_map_size      An entry map size.
    \param p_connection_map    A connection map.
    \param connection_map_size A connection map size.

    \return A pointer to an unordered_map vocabulary. Or NULL when p_entry_map is NULL.
*/
tetengo_lattice_vocabulary* tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
    const tetengo_lattice_entry_map_element*      p_entry_map,
    size_t                                        entry_map_size,
    const tetengo_lattice_connection_map_element* p_connection_map,
    size_t                                        connection_map_size);

/*!
    \brief Destroys the vocabulary.

    \param p_vocabulary A pointer to a vocabulary.
*/
void tetengo_lattice_vocabulary_destroy(const tetengo_lattice_vocabulary* p_vocabulary);

/*!
    \brief Finds entries.

    \param p_vocabulary A pointer to a vocabulary.
    \param key          A key.
    \param p_entries    The storage for output entries. Can be NULL.

    \return An entry count.
*/
size_t tetengo_lattice_vocabulary_findEntries(
    const tetengo_lattice_vocabulary* p_vocabulary,
    const char*                       key,
    tetengo_lattice_entry*            p_entries);

/*!
    \brief Finds a connection between entries.

    \param p_from       A pointer to an origin node.
    \param p_to         A pointer to a destination node.
    \param p_connection The storage for an output connection.

    \retval non-zero When an output connection is stored.
    \retval 0        Otherwise.
*/
int tetengo_lattice_vocabulary_findConnection(
    const tetengo_lattice_vocabulary* p_vocabulary,
    const tetengo_lattice_entry*      p_from,
    const tetengo_lattice_entry*      p_to,
    tetengo_lattice_connection*       p_connection);


#if defined(__cplusplus)
}
#endif


#endif
