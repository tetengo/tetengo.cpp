/*! \file
    \brief A trie.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_TRIE_H)
#define TETENGO_TRIE_TRIE_H
#if defined(__cplusplus)
extern "C" {
#endif

/*!
    \brief A trie.
*/
struct tetengo_trie_trie;

/*!
    \brief Creates a trie.

    \return A pointer to a trie.
*/
tetengo_trie_trie* tetengo_trie_trie_create();

/*!
    \brief Destroys a trie.

    \param p_trie A pointer to a trie.
*/
void tetengo_trie_trie_destroy(tetengo_trie_trie* p_trie);


#if defined(__cplusplus)
}
#endif
#endif
