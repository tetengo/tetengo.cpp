/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_VOCABULARY_H)
#define TETENGO_LATTICE_VOCABULARY_H

#include <stddef.h>

#include <tetengo/lattice/connection.h>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/input.h>
#include <tetengo/lattice/node.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_vocabulary_tag tetengo_lattice_vocabulary_t;
#endif


/*!
    \brief Creates an unordered map vocabulary.

    \param p_entries        A pointer to entries.
    \param entry_count      An entry count.
    \param p_connections    A pointer to connections.
    \param connection_count A connection count.
    \param p_entry_hash     A pointer to a hash function for an entry.
    \param p_entry_equal_to A pointer to an eqaul_to function for an entry.

    \return A pointer to an unordered map vocabulary.
            Or NULL when p_entries and/or p_connections are NULL, and entry_count and/or connection_count are greater
            than 0.
*/
tetengo_lattice_vocabulary_t* tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
    const tetengo_lattice_keyEntriesPair_t*            p_entries,
    size_t                                             entry_count,
    const tetengo_lattice_entriesConnectionCostPair_t* p_connections,
    size_t                                             connection_count,
    size_t (*p_entry_hash)(const tetengo_lattice_entryView_t*),
    int (*p_entry_equal_to)(const tetengo_lattice_entryView_t*, const tetengo_lattice_entryView_t*));

/*!
    \brief A custom vocabulary definition.
*/
typedef struct tetengo_lattice_customVocabularyDefinition_tag
{
    /*! The id. */
    int id;

    /*! The pointer to the context. */
    void* p_context;

    /*!
        \brief The procedure for findEntries.

        \param p_context A pointer to the context.
        \param p_key     A pointer to a key.
        \param p_entries The storage for output entries. Can be NULL.

        \return An entry count.
    */
    size_t (*find_entries_proc)(
        void*                          p_context,
        const tetengo_lattice_input_t* p_key,
        tetengo_lattice_entryView_t*   p_entries);

    /*!
        \brief The procedure for findConnection.

        \param p_context    A pointer to the context.
        \param p_from       A pointer to an origin node.
        \param p_to         A pointer to a destination entry.
        \param p_connection The storage for an output connection.

        \retval non-zero When an output connection is stored.
        \retval 0        Otherwise.
    */
    int (*find_connection_proc)(
        void*                              p_context,
        const tetengo_lattice_node_t*      p_from,
        const tetengo_lattice_entryView_t* p_to,
        tetengo_lattice_connection_t*      p_connection);

} tetengo_lattice_customVocabularyDefinition_t;

/*!
    \brief Creates a custom vocabulary.

    \param p_definition A pointer to a definition.
    \param p_context    A pointer to a context.

    \return A pointer to a custom vocabulary. Or NULL when p_definition NULL
*/
tetengo_lattice_vocabulary_t* tetengo_lattice_vocabulary_createCustomVocabulary(
    const tetengo_lattice_customVocabularyDefinition_t* p_definition,
    void*                                               p_context);

/*!
    \brief Destroys a vocabulary.

    \param p_vocabulary A pointer to a vocabulary.
*/
void tetengo_lattice_vocabulary_destroy(const tetengo_lattice_vocabulary_t* p_vocabulary);

/*!
    \brief Finds entries.

    \param p_vocabulary A pointer to a vocabulary.
    \param p_key        A pointer to a key.
    \param p_entries    The storage for output entries. Can be NULL.

    \return An entry count.
*/
size_t tetengo_lattice_vocabulary_findEntries(
    const tetengo_lattice_vocabulary_t* p_vocabulary,
    const tetengo_lattice_input_t*      p_key,
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
