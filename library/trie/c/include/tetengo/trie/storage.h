/*! \file
    \brief A storage.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_STORAGE_H)
#define TETENGO_TRIE_STORAGE_H
#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h>
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

/*!
    \brief Returns the size.

    \param p_storage A pointer to a storage.

    \return The size.
*/
size_t tetengo_trie_storage_size(const tetengo_trie_storage* p_storage);

/*!
    \brief Returns the filling rate.

    \param p_storage A pointer to a storage.

    \return The filling rate.
*/
double tetengo_trie_storage_fillingRate(const tetengo_trie_storage* p_storage);

/*!
    \brief Serializes the storage.

    \param p_storage A pointer to a storage.
    \param path      A file path.
*/
void tetengo_trie_storage_serialize(const tetengo_trie_storage* p_storage, const path_character_type* path);


#if defined(__cplusplus)
}
#endif
#endif
