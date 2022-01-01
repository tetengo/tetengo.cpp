/*! \file
    \brief A trie.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_TRIE_H)
#define TETENGO_TRIE_TRIE_H

#include <stdbool.h>
#include <stddef.h>


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
#if !defined(TYPEDEF_TETENGO_TRIE_TRIEITERATOR_T)
#define TYPEDEF_TETENGO_TRIE_TRIEITERATOR_T
typedef struct tetengo_trie_trieIterator_tag tetengo_trie_trieIterator_t;
#endif
#endif


/*!
    \brief An element type.
*/
typedef struct tetengo_trie_trieElement_tag
{
    /*! The key. */
    const char* key;

    /*! The pointer to the value. */
    const void* p_value;
} tetengo_trie_trieElement_t;

/*!
    \brief An observer type called when a key is adding.

    \param serialized_key A serialized key.
    \param p_context      A pointer to the context.
*/
typedef void (*tetengo_trie_trie_addingObserver_t)(const char* serialized_key, void* p_context);

/*!
    \brief A null adding observer.
*/
void tetengo_trie_trie_nullAddingObserver(const char*, void*);

/*!
    \brief An observer type called when the building is done.

    \param p_context A pointer to the context.
*/
typedef void (*tetengo_trie_trie_doneObserver_t)(void* p_context);

/*!
    \brief A null done observer.
*/
void tetengo_trie_trie_nullDoneObserver(void*);

/*!
    \brief Returns the default double array density factor.

    \return The default double array density factor.
*/
size_t tetengo_trie_trie_defaultDoubleArrayDensityFactor(void);

/*!
    \brief Creates a trie.

    \param p_elements                  A pointer to the first element.
    \param element_count               An element count.
    \param element_value_size          An element value size.
    \param adding_observer             An adding observer.
    \param p_adding_observer_context   A pointer to the adding observer context.
    \param done_observer               A done observer.
    \param p_done_observer_context     A pointer to the done observer context.
    \param double_array_density_factor A double array density factor.

    \return A pointer to a trie. Or NULL on error.
*/
tetengo_trie_trie_t* tetengo_trie_trie_create(
    const tetengo_trie_trieElement_t*  p_elements,
    size_t                             element_count,
    size_t                             element_value_size,
    tetengo_trie_trie_addingObserver_t adding_observer,
    void*                              p_adding_observer_context,
    tetengo_trie_trie_doneObserver_t   done_observer,
    void*                              p_done_observer_context,
    size_t                             double_array_density_factor);

/*!
    \brief Creates a trie.

    The ownership of the storage pointed by p_storage is transferred into the trie.
    There is no need to destroy the storage after calling this function.

    \param p_storage A pointer to a storage.

    \return A pointer to a trie. Or NULL on error or when p_storage does not own a storage but just refferes one.
*/
tetengo_trie_trie_t* tetengo_trie_trie_createWithStorage(tetengo_trie_storage_t* p_storage);

/*!
    \brief Destroys a trie.

    \param p_trie A pointer to a trie.
*/
void tetengo_trie_trie_destroy(const tetengo_trie_trie_t* p_trie);

/*!
    \brief Returns true when the trie is empty.

    \param p_trie A pointer to a trie.

    \retval true  When the trie is empty.
    \retval false Otherwise.
*/
bool tetengo_trie_trie_empty(const tetengo_trie_trie_t* p_trie);

/*!
    \brief Returns the size of the trie.

    \param p_trie A pointer to a trie.

    \return The size. Or (size_t)-1 on error.
*/
size_t tetengo_trie_trie_size(const tetengo_trie_trie_t* p_trie);

/*!
    \brief Returns true when the trie contains the given key.

    \param p_trie A pointer to a trie.
    \param key    A key.

    \retval true  When the trie contains the given key.
    \retval false Otherwise.
*/
bool tetengo_trie_trie_contains(const tetengo_trie_trie_t* p_trie, const char* key);

/*!
    \brief Finds the value object correspoinding the given key.

    \param p_trie A pointer to a trie.
    \param key    A key.

    \return A pointer to the value object. Or NULL on error or when the trie does not have the given key.
*/
const void* tetengo_trie_trie_find(const tetengo_trie_trie_t* p_trie, const char* key);

/*!
    \brief Creates an iterator.

    \param p_trie A pointer to a trie.

    \return A pointer to an iterator. Or NULL on error.
*/
tetengo_trie_trieIterator_t* tetengo_trie_trie_createIterator(const tetengo_trie_trie_t* p_trie);

/*!
    \brief Destroys an iterator.

    \param p_iterator A pointer to an iterator.
*/
void tetengo_trie_trie_destroyIterator(const tetengo_trie_trieIterator_t* p_iterator);

/*!
    \brief Creates a subtrie.

    \param p_trie     A pointer to a trie.
    \param key_prefix A key prefix.

    \return A pointer to a subtrie. Or NULL on error or when the trie does not have the given key prefix.
*/
const tetengo_trie_trie_t* tetengo_trie_trie_subtrie(const tetengo_trie_trie_t* p_trie, const char* key_prefix);

/*!
    \brief Returns the pointer to the storage.

    \param p_trie A pointer to a trie.

    \return The pointer to the storage. Or NULL on error.
*/
const tetengo_trie_storage_t* tetengo_trie_trie_getStorage(const tetengo_trie_trie_t* p_trie);


#if defined(__cplusplus)
}
#endif


#endif
