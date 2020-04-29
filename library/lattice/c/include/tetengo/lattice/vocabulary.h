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
typedef struct tetengo_lattice_vocabulary_tag        tetengo_lattice_vocabulary;
typedef struct tetengo_lattice_entry_tag             tetengo_lattice_entry;
typedef struct tetengo_lattice_entry_map_element_tag tetengo_lattice_entry_map_element;
#endif


/*!
    \brief Creates an unordered_map vocabulary.

    \param p_map    An entry map.
    \param map_size An entry map size.

    \return A pointer to an unordered_map vocabulary. Or NULL when p_map is NULL.
*/
tetengo_lattice_vocabulary* tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
    const tetengo_lattice_entry_map_element* p_map,
    size_t                                   map_size);

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


#if defined(__cplusplus)
}
#endif


#endif
