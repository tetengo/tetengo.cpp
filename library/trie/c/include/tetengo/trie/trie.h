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

    //! The pointer to the element.
    const void* p_element;
};

/*!
    \brief An observer type called when a key is adding.

    \param serialized_key A serialized key.
*/
typedef void (*tetengo_trie_trie_addingObserver_t)(const char* serialized_key);

/*!
    \brief A null adding observer.
*/
void tetengo_trie_trie_nullAddingObserver(const char*);

/*!
    \brief An observer type called when the building is done.
*/
typedef void (*tetengo_trie_trie_doneObserver_t)();

/*!
    \brief A null done observer.
*/
void tetengo_trie_trie_nullDoneObserver();

/*!
    \brief Returns the default double array density factor.

    \return The default double array density factor.
*/
size_t tetengo_trie_trie_defaultDoubleArrayDensityFactor();

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
