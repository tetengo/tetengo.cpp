/*! \file
    \brief A constraint element.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_CONSTRAINTELEMENT_H)
#define TETENGO_LATTICE_CONSTRAINTELEMENT_H

#include <stddef.h>

#include <tetengo/lattice/node.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_constraintElement_tag tetengo_lattice_constraintElement_t;
#endif


/*!
    \brief Creates a node constraint element.

    \param p_node A pointer to a node.

    \return A pointer to a node constraint element. Or NULL when p_node is NULL.
*/
tetengo_lattice_constraintElement_t*
tetengo_lattice_constraintElement_createNodeConstraintElement(const tetengo_lattice_node_t* p_node);

/*!
    \brief Creates a node constraint element.

    \param preceding_step An index of a preceding step.

    \return A pointer to a node constraint element. Or NULL when p_node is NULL.
*/
tetengo_lattice_constraintElement_t*
tetengo_lattice_constraintElement_createWildcardConstraintElement(size_t preceding_step);

/*!
    \brief Destroys a constraint element.

    \param p_constraint_element A pointer to a constraint element.
*/
void tetengo_lattice_constraintElement_destroy(const tetengo_lattice_constraintElement_t* p_constraint_element);

/*!
    \brief Returns whether this constraint element matches the specified node.

    \param p_constraint_element A pointer to a constraint element.
    \param p_node               A pointer to a node.

    \retval positive This constraint element matches the specified node, and also may match its preceding nodes.
    \retval 0        This constraint element matches the specified node, and do not match its precedings nodes.
    \retval negative This constraint element does not match the specified node.
                     Or p_constraint_element and/or p_node are NULL.
*/
int tetengo_lattice_constraintElement_matches(
    const tetengo_lattice_constraintElement_t* p_constraint_element,
    const tetengo_lattice_node_t*              p_node);


#if defined(__cplusplus)
}
#endif


#endif
