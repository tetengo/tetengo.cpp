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
    \param adding_observer             An adding observer.
    \param p_adding_observer_context   A pointer to the adding observer context.
    \param done_observer               A done observer.
    \param p_done_observer_context     A pointer to the done observer context.
    \param double_array_density_factor A double array density factor.

    \return A pointer to a trie.
*/
tetengo_trie_trie* tetengo_trie_trie_create(
    const tetengo_trie_trie_element_t* p_elements,
    size_t                             element_count,
    tetengo_trie_trie_addingObserver_t adding_observer,
    void*                              p_adding_observer_context,
    tetengo_trie_trie_doneObserver_t   done_observer,
    void*                              p_done_observer_context,
    size_t                             double_array_density_factor);

/*!
    \brief Destroys a trie.

    \param p_trie A pointer to a trie.
*/
void tetengo_trie_trie_destroy(tetengo_trie_trie* p_trie);

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

    \return The size.
*/
size_t tetengo_trie_trie_size(const tetengo_trie_trie* p_trie);


#if defined(__cplusplus)
}
#endif
#endif
