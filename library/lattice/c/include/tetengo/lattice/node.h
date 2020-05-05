/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_LATTICE_NODE_H)
#define TETENGO_LATTICE_NODE_H

#include <stddef.h>

#include <tetengo/lattice/string_view.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_entry_tag tetengo_lattice_entry_t;
#endif


/*!
    \brief A node.
*/
typedef struct tetengo_lattice_node_tag
{
    //! A key.
    tetengo_lattice_stringView_t key;

    //! A pointer to a value.
    const void* p_value;

    //! An index of preceding step.
    size_t preceding_step;

    //! A node cost.
    int node_cost;

    //! A path cost.
    int path_cost;


} tetengo_lattice_node_t;

/*!
    \brief Returns the pointer to the BOS (Beginning of Sequence).

    \return The pointer to the BOS.
*/
const tetengo_lattice_node_t* tetengo_lattice_node_bos();

/*!
    \brief Returns an EOS (End of Sequence).

    \param preceding_step An index of preceding step.
    \param path_cost      A path cost.
    \param p_eos          The storage for an output EOS.

    \retval non-zero When an EOS is stored.
    \retval 0        Otherwise.
*/
int tetengo_lattice_node_eos(size_t preceding_step, int path_cost, tetengo_lattice_node_t* p_eos);

/*!
    \brief Makes a node from an entry.

    \param p_entry        A pointer to an entry.
    \param preceding_step An index of preceding step.
    \param path_cost      A path cost.
    \param p_node         The storage for an output node.

    \retval non-zero When a node is stored.
    \retval 0        Otherwise.
*/
int tetengo_lattice_node_toNode(
    const tetengo_lattice_entry_t* p_entry,
    size_t                         preceding_step,
    int                            path_cost,
    tetengo_lattice_node_t*        p_node);


#if defined(__cplusplus)
}
#endif


#endif
