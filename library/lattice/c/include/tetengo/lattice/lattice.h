/*! \file
    \brief A lattice.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_LATTICE_H)
#define TETENGO_LATTICE_LATTICE_H

#include <stdbool.h>
#include <stddef.h>

#include <tetengo/lattice/input.h>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/vocabulary.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_lattice_tag tetengo_lattice_lattice_t;
#endif


/*!
    \brief Creates a lattice.

    \param p_vocabulary A pointer to a vocabulary.

    \return A pointer to a lattice. Or NULL when p_vocabulary is NULL.
*/
tetengo_lattice_lattice_t* tetengo_lattice_lattice_create(const tetengo_lattice_vocabulary_t* p_vocabulary);

/*!
    \brief Destroys a lattice.

    \param p_lattice A pointer to a lattice.
*/
void tetengo_lattice_lattice_destroy(const tetengo_lattice_lattice_t* p_lattice);

/*!
    \brief Returns the step count.

    \param p_lattice A pointer to a lattice.

    \return The step count. Or 0 when p_lattice is NULL.
*/
size_t tetengo_lattice_lattice_stepCount(const tetengo_lattice_lattice_t* p_lattice);

/*!
    \brief Returns the nodes at the specified step.

    \param p_lattice A pointer to a lattice.
    \param step      A step.
    \param p_nodes   The storage for output nodes. Can be NULL.

    \return A node count. Or 0 when p_lattice is NULL or step is too large.
*/
size_t tetengo_lattice_lattice_nodesAt(
    const tetengo_lattice_lattice_t* p_lattice,
    size_t                           step,
    tetengo_lattice_node_t*          p_nodes);

/*!
    \brief Pushes back an input.

    The ownership of the input pointed by p_input is transferred into the lattice.
    There is no need to destroy the input after calling this function.

    \param p_lattice A pointer to a lattice.
    \param p_input   A pointer to an input.

    \retval true  When the input is pushed back.
    \retval false Otherwise.
*/
bool tetengo_lattice_lattice_pushBack(tetengo_lattice_lattice_t* p_lattice, tetengo_lattice_input_t* p_input);

/*!
    \brief Settles this lattice.

    You can modify the lattice after settlement.
    Modification of the lattice after settlement invalidate the EOS node.

    \param p_lattice              A pointer to a lattice.
    \param p_eos_node             The storage for an output EOS node. Can be NULL.
    \param p_preceding_edge_costs The storage for preceding edge costs of the EOS. Can be NULL.

    \return The preceding edge cost count.
*/
size_t tetengo_lattice_lattice_settle(
    tetengo_lattice_lattice_t* p_lattice,
    tetengo_lattice_node_t*    p_eos_node,
    int*                       p_preceding_edge_costs);


#if defined(__cplusplus)
}
#endif


#endif
