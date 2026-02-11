/*! \file
    \brief An N-best lattice path iterator.

    $12026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_NBESTITERATOR_H)
#define TETENGO_LATTICE_NBESTITERATOR_H

#include <stdbool.h>

#include <tetengo/lattice/constraint.h>
#include <tetengo/lattice/lattice.h>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/path.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_nBestIterator_tag tetengo_lattice_nBestIterator_t;
#endif


/*!
    \brief Creates an iterator.

    The ownership of the constraint pointed by p_constraint is transferred into the iterator.
    There is no need to destroy the constraint after calling this function.

    \param p_lattice    A pointer to a lattice.
    \param p_eos_node   A pointer to an EOS node.
    \param p_constraint A pointer to a constraint.

    \return A pointer to an iterator. Or NULL on error.
*/
tetengo_lattice_nBestIterator_t* tetengo_lattice_nBestIterator_create(
    const tetengo_lattice_lattice_t* p_lattice,
    const tetengo_lattice_node_t*    p_eos_node,
    tetengo_lattice_constraint_t*    p_constraint);

/*!
    \brief Destroys an iterator.

    \param p_iterator A pointer to an iterator.
*/
void tetengo_lattice_nBestIterator_destroy(const tetengo_lattice_nBestIterator_t* p_iterator);

/*!
    \brief Dereferences the iterator.

    \param p_iterator A pointer to an iterator.

    \return A pointer to the path. Or NULL when p_iterator is NULL.
*/
tetengo_lattice_path_t* tetengo_lattice_nBestIterator_createPath(const tetengo_lattice_nBestIterator_t* p_iterator);

/*!
    \brief Returns true when the iterator will return more elements.

    \param p_iterator A pointer to an iterator.

    \retval true  When the iterator will return more elements.
    \retval false Otherwise.
*/
bool tetengo_lattice_nBestIterator_hasNext(const tetengo_lattice_nBestIterator_t* p_iterator);

/*!
    \brief Increments the iterator.

    \param p_iterator A pointer to an iterator.
*/
void tetengo_lattice_nBestIterator_next(tetengo_lattice_nBestIterator_t* p_iterator);


#if defined(__cplusplus)
}
#endif


#endif
