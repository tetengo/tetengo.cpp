/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_LATTICE_NBESTITERATOR_H)
#define TETENGO_LATTICE_NBESTITERATOR_H

#include <stddef.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_nBestIterator_tag tetengo_lattice_nBestIterator_t;
typedef struct tetengo_lattice_lattice_tag       tetengo_lattice_lattice_t;
typedef struct tetengo_lattice_node_tag          tetengo_lattice_node_t;
#endif


/*!
    \brief Creates an iterator.

    \param p_lattice  A pointer to a lattice.
    \param p_eos_node A pointer to an EOS node.

    \return A pointer to an iterator. Or NULL on error.
*/
tetengo_lattice_nBestIterator_t* tetengo_lattice_nBestIterator_create(
    const tetengo_lattice_lattice_t* p_lattice,
    const tetengo_lattice_node_t*    p_eos_node);

/*!
    \brief Destroys an iterator.

    \param p_iterator A pointer to an iterator.
*/
void tetengo_lattice_nBestIterator_destroy(const tetengo_lattice_nBestIterator_t* p_iterator);

/*!
    \brief Dereferences the iterator.

    \param p_iterator A pointer to an iterator.
    \param p_path     The storage for an output path. Can be NULL.

    \return The node count of the path.
*/
size_t
tetengo_lattice_nBestIterator_get(const tetengo_lattice_nBestIterator_t* p_iterator, tetengo_lattice_node_t* p_path);

/*!
    \brief Returns non-zero when the iterator will return more elements.

    \param p_iterator A pointer to an iterator.

    \retval non-zero When the iterator will return more elements.
    \retval 0        Otherwise.
*/
int tetengo_lattice_nBestIterator_hasNext(const tetengo_lattice_nBestIterator_t* p_iterator);

/*!
    \brief Increments the iterator.

    \param p_iterator A pointer to an iterator.
*/
void tetengo_lattice_nBestIterator_next(tetengo_lattice_nBestIterator_t* p_iterator);


#if defined(__cplusplus)
}
#endif


#endif
