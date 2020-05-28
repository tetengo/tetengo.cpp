/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_LATTICE_NODE_H)
#define TETENGO_LATTICE_NODE_H

#include <stddef.h>

#include <tetengo/lattice/stringView.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_entryView_tag                tetengo_lattice_entryView_t;
typedef const struct tetengo_lattice_entry_valueHandle_tag* tetengo_lattice_entry_valueHandle_t;
#endif


/*!
    \brief A node.
*/
typedef struct tetengo_lattice_node_tag
{
    //! A key.
    tetengo_lattice_stringView_t key;

    //! A value handle.
    tetengo_lattice_entry_valueHandle_t value_handle;

    //! An index of a preceding step.
    size_t preceding_step;

    //! A pointer to preceding edge costs.
    const int* p_preceding_edge_costs;

    //! A preceding edge cost count.
    size_t preceding_edge_cost_count;

    //! An index of a best preceding node.
    size_t best_preceding_node;

    //! A node cost.
    int node_cost;

    //! A path cost.
    int path_cost;


} tetengo_lattice_node_t;

/*!
    \brief Returns the pointer to the BOS (Beginning of Sequence).

    \param p_preceding_edge_costs A pointer to preceding edge costs.
    \param preceding_edge_count   A preceding edge count.
    \param p_bos                  The storage for an output BOS.

    \retval non-zero When an EOS is stored.
    \retval 0        Otherwise.
*/
int tetengo_lattice_node_bos(
    const int*              p_preceding_edge_costs,
    size_t                  preceding_edge_count,
    tetengo_lattice_node_t* p_bos);

/*!
    \brief Returns an EOS (End of Sequence).

    \param preceding_step         An index of a preceding step.
    \param p_preceding_edge_costs A pointer to preceding edge costs.
    \param preceding_edge_count   A preceding edge count.
    \param best_preceding_node    An index of a best preceding node.
    \param path_cost              A path cost.
    \param p_eos                  The storage for an output EOS.

    \retval non-zero When an EOS is stored.
    \retval 0        Otherwise.
*/
int tetengo_lattice_node_eos(
    size_t                  preceding_step,
    const int*              p_preceding_edge_costs,
    size_t                  preceding_edge_count,
    size_t                  best_preceding_node,
    int                     path_cost,
    tetengo_lattice_node_t* p_eos);

/*!
    \brief Makes a node from an entry.

    \param p_entry                A pointer to an entry.
    \param preceding_step         An index of a preceding step.
    \param p_preceding_edge_costs A pointer to preceding edge costs.
    \param preceding_edge_count   A preceding edge count.
    \param best_preceding_node    An index of a best preceding node.
    \param path_cost              A path cost.
    \param p_node                 The storage for an output node.

    \retval non-zero When a node is stored.
    \retval 0        Otherwise.
*/
int tetengo_lattice_node_toNode(
    const tetengo_lattice_entryView_t* p_entry,
    size_t                             preceding_step,
    const int*                         p_preceding_edge_costs,
    size_t                             preceding_edge_count,
    size_t                             best_preceding_node,
    int                                path_cost,
    tetengo_lattice_node_t*            p_node);

/*!
    \brief Returns true if one node is equal to another.

    \param p_one     A pointer to one node.
    \param p_another A pointer to another node.

    \retval non-zero When one node is equal to another.
    \retval 0        Otherwise.
*/
int tetengo_lattice_node_equal(const tetengo_lattice_node_t* p_one, const tetengo_lattice_node_t* p_another);

/*!
    \brief Returns true is this node is the BOS.

    \param p_node A pointer to node.

    \retval non-zero When this node is the BOS.
    \retval 0        Otherwise.
*/
int tetengo_lattice_node_isBos(const tetengo_lattice_node_t* p_node);


#if defined(__cplusplus)
}
#endif


#endif
