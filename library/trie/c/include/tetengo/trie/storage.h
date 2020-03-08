/*! \file
    \brief A storage.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_STORAGE_H)
#define TETENGO_TRIE_STORAGE_H
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(_WIN32)
#include <wchar.h>
#endif

struct tetengo_trie_trie;


/*!
    \brief A storage.
*/
struct tetengo_trie_storage;

/*!
    \brief A path character type.
*/
#if defined(_WIN32)
typedef wchar_t path_character_type;
#else
typedef char path_character_type;
#endif

/*!
    \brief Creates a storage.

    \param p_trie A pointer to a trie.

    \return A pointer to a storage.
*/
tetengo_trie_storage* tetengo_trie_storage_createStorage(const tetengo_trie_trie* p_trie);

/*!
    \brief Creates a memory storage.

    \param path A file path in which content is stored.

    \return A pointer to a memory storage. Or NULL when content cannot be loaded from the path.
*/
tetengo_trie_storage* tetengo_trie_storage_createMemoryStorage(const path_character_type* path);

/*!
    \brief Destroys a storage.

    \param p_storage A pointer to a storage.
*/
void tetengo_trie_storage_destroy(const tetengo_trie_storage* p_storage);


#if defined(__cplusplus)
}
#endif
#endif
