/*! \file
    \brief A storage.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_STORAGE_H)
#define TETENGO_TRIE_STORAGE_H
#if defined(__cplusplus)
extern "C" {
#endif


/*!
    \brief A storage.
*/
struct tetengo_trie_storage;

/*!
    \brief Creates a memory storage.

    \return A pointer to a memory storage.
*/
tetengo_trie_storage* tetengo_trie_storage_createMemoryStorage();

/*!
    \brief Destroys a storage.

    \param p_storage A pointer to a storage.
*/
void tetengo_trie_storage_destroy(const tetengo_trie_storage* p_storage);


#if defined(__cplusplus)
}
#endif
#endif
