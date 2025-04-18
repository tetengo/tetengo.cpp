/*! \file
    \brief A storage.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_STORAGE_H)
#define TETENGO_TRIE_STORAGE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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
    \brief Returns the check value for a vacant element.

    \return The check value for a vacant element.
*/
uint8_t tetengo_trie_storage_vacantCheckValue();

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
    \brief Creates an mmap storage.

    \param path           A file path in which content is stored.
    \param content_offset A content offset in the file of the path.

    \return A pointer to an mmap storage.
            Or NULL when content cannot be loaded from the path, content_offset is greater than the file size, or the
            value size is not fixed.
*/
tetengo_trie_storage_t* tetengo_trie_storage_createMmapStorage(const path_character_type* path, size_t content_offset);

/*!
    \brief Destroys a storage.

    \param p_storage A pointer to a storage.
*/
void tetengo_trie_storage_destroy(const tetengo_trie_storage_t* p_storage);

/*!
    \brief Returns the base-check size.

    \param p_storage A pointer to a storage.

    \return The base-check size. Or (size_t)-1 on error.
*/
size_t tetengo_trie_storage_baseCheckSize(const tetengo_trie_storage_t* p_storage);

/*!
    \brief Returns the base value.

    \param p_storage        A pointer to a storage.
    \param base_check_index A base-check index.

    \return The base value. Or INT32_MAX on error.
*/
int32_t tetengo_trie_storage_baseAt(const tetengo_trie_storage_t* p_storage, size_t base_check_index);

/*!
    \brief Sets a base value.

    \param p_storage        A pointer to a storage.
    \param base_check_index A base-check index.
    \param base             A base value.

    \retval true  On no error.
    \retval false Otherwise.
*/
bool tetengo_trie_storage_setBaseAt(tetengo_trie_storage_t* p_storage, size_t base_check_index, int32_t base);

/*!
    \brief Returns the check value.

    \param p_storage        A pointer to a storage.
    \param base_check_index A base-check index.

    \return The check value. Or UINT8_MAX on error.
*/
uint8_t tetengo_trie_storage_checkAt(const tetengo_trie_storage_t* p_storage, size_t base_check_index);

/*!
    \brief Sets a check value.

    \param p_storage        A pointer to a storage.
    \param base_check_index A base-check index.
    \param check            A check value.

    \retval true  On no error.
    \retval false Otherwise.
*/
bool tetengo_trie_storage_setCheckAt(tetengo_trie_storage_t* p_storage, size_t base_check_index, uint8_t check);

/*!
    \brief Returns the value count.

    \param p_storage A pointer to a storage.

    \return The value count. Or (size_t)-1 on error.
*/
size_t tetengo_trie_storage_valueCount(const tetengo_trie_storage_t* p_storage);

/*!
    \brief Returns the value object.

    \param p_storage   A pointer to a storage.
    \param value_index A value index.

    \return A pointer to the value object. Or NULL when there is no corresponding value object.
*/
const void* tetengo_trie_storage_valueAt(const tetengo_trie_storage_t* p_storage, size_t value_index);

/*!
    \brief Adds a value object.

    \param p_storage   A pointer to a storage.
    \param value_index A value index.
    \param p_value     A pointer to a value object. Must not be NULL.
    \param value_size  A value size.

    \retval true  On no error.
    \retval false Otherwise.
*/
bool tetengo_trie_storage_addValueAt(
    tetengo_trie_storage_t* p_storage,
    size_t                  value_index,
    const void*             p_value,
    size_t                  value_size);

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

    \retval true  On no error.
    \retval false Otherwise.
*/
bool tetengo_trie_storage_serialize(
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
