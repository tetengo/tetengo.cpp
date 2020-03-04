/*! \file
    \brief A trie iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_TRIEITERATOR_H)
#define TETENGO_TRIE_TRIEITERATOR_H
#if defined(__cplusplus)
extern "C" {
#endif

struct tetengo_trie_trie;


/*!
    \brief A trie iterator.
*/
struct tetengo_trie_trieIterator;

/*!
    \brief Creates an iterator.

    \param p_trie A pointer to a trie.

    \return A pointer to an iterator.
*/
tetengo_trie_trieIterator* tetengo_trie_trieIterator_create(const tetengo_trie_trie* p_trie);

/*!
    \brief Destroys an iterator.

    \param p_iterator A pointer to a iterator.
*/
void tetengo_trie_trieIterator_destroy(const tetengo_trie_trieIterator* p_iterator);

/*!
    \brief Dereferences the iterator.

    \param p_iterator A pointer to a iterator.

    \return The dereferenced value.
*/
const void* tetengo_trie_trieIterator_get(const tetengo_trie_trieIterator* p_iterator);

/*!
    \brief Returns non-zero when the iterator will return more elements.

    \param p_iterator A pointer to a iterator.

    \retval non-zero When the iterator will return more elements.
    \retval 0        Otherwise.
*/
int tetengo_trie_trieIterator_hasNext(const tetengo_trie_trieIterator* p_iterator);

/*!
    \brief Increments the iterator.

    \param p_iterator A pointer to a iterator.
*/
void tetengo_trie_trieIterator_next(tetengo_trie_trieIterator* p_iterator);


#if defined(__cplusplus)
}
#endif
#endif
