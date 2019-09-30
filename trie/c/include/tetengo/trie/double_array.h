/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_DOUBLEARRAY_H)
#define TETENGO_TRIE_DOUBLEARRAY_H
#if defined(__cplusplus)
extern "C" {
#endif

/*!
    \brief A double array.
*/
struct tetengo_trie_doublearray;

/*!
    \brief Creates a double array.

    \return A pointer to a double array.
*/
tetengo_trie_doublearray* tetengo_trie_doublearray_create();

/*!
    \brief Destroys a double array.

    \param p_double_array A pointer to a double array.
*/
void tetengo_trie_doublearray_destroy(tetengo_trie_doublearray* p_double_array);


#if defined(__cplusplus)
}
#endif
#endif
