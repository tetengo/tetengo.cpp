/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_LATTICE_NBESTITERATOR_H)
#define TETENGO_LATTICE_NBESTITERATOR_H

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_nBestIterator_tag tetengo_lattice_nBestIterator_t;
typedef struct tetengo_lattice_lattice_tag       tetengo_lattice_lattice_t;
typedef struct tetengo_lattice_node_tag          tetengo_lattice_node_t;
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
    \brief Creates an iterator.

    \param p_lattice  A pointer to a lattice.
    \param p_eos_node A pointer to an EOS node.

    \return A pointer to an iterator.
*/
tetengo_lattice_nBestIterator_t* tetengo_lattice_nBestIterator_createBegin(
    const tetengo_lattice_lattice_t* p_lattice,
    const tetengo_lattice_node_t*    p_eos_node);


/*!
    \brief Destroys an iterator.

    \param p_iterator A pointer to an iterator.
*/
void tetengo_lattice_nBestIterator_destroy(const tetengo_lattice_nBestIterator_t* p_iterator);


#if defined(__cplusplus)
}
#endif


#endif
