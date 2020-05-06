/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_LATTICE_NBESTITERATOR_H)
#define TETENGO_LATTICE_NBESTITERATOR_H

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_nBestIterator_tag tetengo_lattice_nBestIterator_t;
#endif


#if defined(__cplusplus)
extern "C" {
#endif

/*!
    \brief Creates an iterator.

    It points to the last of the paths.

    \return A pointer to an iterator.
*/
tetengo_lattice_nBestIterator_t* tetengo_lattice_nBestIterator_createEnd();

/*!
    \brief Destroys the iterator.

    \param p_iterator A pointer to an iterator.
*/
void tetengo_lattice_nBestIterator_destroy(const tetengo_lattice_nBestIterator_t* p_iterator);


#if defined(__cplusplus)
}
#endif


#endif
