/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_STORAGE_H)
#define TETENGO_TRIE_STORAGE_H

#include <stddef.h>
#if defined(_WIN32)
#include <wchar.h>
#endif


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
#if !defined(TYPEDEF_TETENGO_TRIE_STORAGE_T)
#define TYPEDEF_TETENGO_TRIE_STORAGE_T
typedef struct tetengo_trie_storage_tag tetengo_trie_storage_t;
#endif
#if !defined(TYPEDEF_TETENGO_TRIE_TRIE_T)
#define TYPEDEF_TETENGO_TRIE_TRIE_T
typedef struct tetengo_trie_trie_tag tetengo_trie_trie_t;
#endif
#endif


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

    \return A pointer to a storage. Or NULL on error.
*/
tetengo_trie_storage_t* tetengo_trie_storage_createStorage(const tetengo_trie_trie_t* p_trie);

/*!
    \brief Creates a memory storage.

    \param path A file path in which content is stored.

    \return A pointer to a memory storage. Or NULL when content cannot be loaded from the path.
*/
tetengo_trie_storage_t* tetengo_trie_storage_createMemoryStorage(const path_character_type* path);

/*!
    \brief Creates a shared storage.

    \param path A file path in which content is stored.

    \return A pointer to a shared storage. Or NULL when content cannot be loaded from the path.
*/
tetengo_trie_storage_t* tetengo_trie_storage_createSharedStorage(const path_character_type* path);

/*!
    \brief Destroys a storage.

    \param p_storage A pointer to a storage.
*/
void tetengo_trie_storage_destroy(const tetengo_trie_storage_t* p_storage);

/*!
    \brief Returns the value size.

    \param p_storage A pointer to a storage.

    \return The value size. Or (size_t)-1 on error.
*/
size_t tetengo_trie_storage_valueSize(const tetengo_trie_storage_t* p_storage);

/*!
    \brief Returns the filling rate.

    \param p_storage A pointer to a storage.

    \return The filling rate. Or NAN on error.
*/
double tetengo_trie_storage_fillingRate(const tetengo_trie_storage_t* p_storage);

/*!
    \brief Serializes the storage.

    \param p_storage        A pointer to a storage.
    \param path             A file path.
    \param fixed_value_size The value size if it is fixed. Or 0 if the size is variable.
*/
void tetengo_trie_storage_serialize(
    const tetengo_trie_storage_t* p_storage,
    const path_character_type*    path,
    size_t                        fixed_value_size);

/*!
    \brief Clones a storage.

    \param p_storage A pointer to a storage.

    \return A pointer to a clone of the storage. Or NULL on error.
*/
tetengo_trie_storage_t* tetengo_trie_storage_clone(const tetengo_trie_storage_t* p_storage);


#if defined(__cplusplus)
}
#endif


#endif
