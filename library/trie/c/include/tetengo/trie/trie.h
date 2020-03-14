/*! \file
    \brief A trie.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_TRIE_H)
#define TETENGO_TRIE_TRIE_H
#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h>

struct tetengo_trie_storage;
struct tetengo_trie_trieIterator;


/*!
    \brief A trie.
*/
struct tetengo_trie_trie;

/*!
    \brief An element type.
*/
struct tetengo_trie_trie_element_t
{
    //! The key.
    const char* key;

    //! The pointer to the value.
    const void* p_value;
};

/*!
    \brief An observer type called when a key is adding.

    \param serialized_key A serialized key.
    \param p_context      A pointer to the context.
*/
typedef void (*tetengo_trie_trie_addingObserver_t)(const char* serialized_key, void* context);

/*!
    \brief A null adding observer.
*/
void tetengo_trie_trie_nullAddingObserver(const char*, void*);

/*!
    \brief An observer type called when the building is done.

    \param p_context A pointer to the context.
*/
typedef void (*tetengo_trie_trie_doneObserver_t)(void* context);

/*!
    \brief A null done observer.
*/
void tetengo_trie_trie_nullDoneObserver(void*);

/*!
    \brief Returns the default double array density factor.

    \return The default double array density factor.
*/
size_t tetengo_trie_trie_defaultDoubleArrayDensityFactor();

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
tetengo_trie_trie* tetengo_trie_trie_create(
    const tetengo_trie_trie_element_t* p_elements,
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
tetengo_trie_trie* tetengo_trie_trie_createWithStorage(tetengo_trie_storage* p_storage);

/*!
    \brief Destroys a trie.

    \param p_trie A pointer to a trie.
*/
void tetengo_trie_trie_destroy(const tetengo_trie_trie* p_trie);

/*!
    \brief Returns non-zero when the trie is empty.

    \param p_trie A pointer to a trie.

    \retval non-zero When the trie is empty.
    \retval 0        Otherwise.
*/
int tetengo_trie_trie_empty(const tetengo_trie_trie* p_trie);

/*!
    \brief Returns the size of the trie.

    \param p_trie A pointer to a trie.

    \return The size. Or (size_t)-1 on error.
*/
size_t tetengo_trie_trie_size(const tetengo_trie_trie* p_trie);

/*!
    \brief Returns non-zero when the trie contains the given key.

    \param p_trie A pointer to a trie.
    \param key    A key.

    \retval non-zero When the trie contains the given key.
    \retval 0        Otherwise.
*/
int tetengo_trie_trie_contains(const tetengo_trie_trie* p_trie, const char* key);

/*!
    \brief Finds the value object correspoinding the given key.

    \param p_trie A pointer to a trie.
    \param key    A key.

    \return A pointer to the value object. Or NULL on error or when the trie does not have the given key.
*/
const void* tetengo_trie_trie_find(const tetengo_trie_trie* p_trie, const char* key);

/*!
    \brief Creates an iterator.

    \param p_trie A pointer to a trie.

    \return A pointer to an iterator. Or NULL on error.
*/
tetengo_trie_trieIterator* tetengo_trie_trie_createIterator(const tetengo_trie_trie* p_trie);

/*!
    \brief Destroys an iterator.

    \param p_iterator A pointer to an iterator.
*/
void tetengo_trie_trie_destroyIterator(const tetengo_trie_trieIterator* p_iterator);

/*!
    \brief Creates a subtrie.

    \param p_trie     A pointer to a trie.
    \param key_prefix A key prefix.

    \return A pointer to a subtrie. Or NULL on error or when the trie does not have the given key prefix.
*/
const tetengo_trie_trie* tetengo_trie_trie_subtrie(const tetengo_trie_trie* p_trie, const char* key_prefix);

/*!
    \brief Returns the pointer to the storage.

    \param p_trie A pointer to a trie.

    \return The pointer to the storage. Or NULL on error.
*/
const tetengo_trie_storage* tetengo_trie_trie_getStorage(const tetengo_trie_trie* p_trie);


#if defined(__cplusplus)
}
#endif
#endif
