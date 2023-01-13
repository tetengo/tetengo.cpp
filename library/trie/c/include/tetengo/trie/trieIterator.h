/*! \file
    \brief A trie iterator.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_TRIEITERATOR_H)
#define TETENGO_TRIE_TRIEITERATOR_H

#include <stdbool.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
#if !defined(TYPEDEF_TETENGO_TRIE_TRIE_T)
#define TYPEDEF_TETENGO_TRIE_TRIE_T
typedef struct tetengo_trie_trie_tag tetengo_trie_trie_t;
#endif
#if !defined(TYPEDEF_TETENGO_TRIE_TRIEITERATOR_T)
#define TYPEDEF_TETENGO_TRIE_TRIEITERATOR_T
typedef struct tetengo_trie_trieIterator_tag tetengo_trie_trieIterator_t;
#endif
#endif


/*!
    \brief Creates an iterator.

    \param p_trie A pointer to a trie.

    \return A pointer to an iterator. Or NULL on error.
*/
tetengo_trie_trieIterator_t* tetengo_trie_trieIterator_create(const tetengo_trie_trie_t* p_trie);

/*!
    \brief Destroys an iterator.

    \param p_iterator A pointer to an iterator.
*/
void tetengo_trie_trieIterator_destroy(const tetengo_trie_trieIterator_t* p_iterator);

/*!
    \brief Dereferences the iterator.

    \param p_iterator A pointer to an iterator.

    \return The dereferenced value. Or NULL on error.
*/
const void* tetengo_trie_trieIterator_get(const tetengo_trie_trieIterator_t* p_iterator);

/*!
    \brief Returns true when the iterator will return more elements.

    \param p_iterator A pointer to an iterator.

    \retval true  When the iterator will return more elements.
    \retval false Otherwise.
*/
bool tetengo_trie_trieIterator_hasNext(const tetengo_trie_trieIterator_t* p_iterator);

/*!
    \brief Increments the iterator.

    \param p_iterator A pointer to an iterator.
*/
void tetengo_trie_trieIterator_next(tetengo_trie_trieIterator_t* p_iterator);


#if defined(__cplusplus)
}
#endif


#endif
