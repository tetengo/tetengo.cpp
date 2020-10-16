/*! \file
    \brief A constraint.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_CONSTRAINT_H)
#define TETENGO_LATTICE_CONSTRAINT_H

#include <stddef.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
#if !defined(TYPEDEF_TETENGO_LATTICE_CONSTRAINT_T)
#define TYPEDEF_TETENGO_LATTICE_CONSTRAINT_T
typedef struct tetengo_lattice_constraint_tag tetengo_lattice_constraint_t;
#endif
#if !defined(TYPEDEF_TETENGO_LATTICE_CONSTRAINTELEMENT_T)
#define TYPEDEF_TETENGO_LATTICE_CONSTRAINTELEMENT_T
typedef struct tetengo_lattice_constraintElement_tag tetengo_lattice_constraintElement_t;
#endif
#if !defined(TYPEDEF_TETENGO_LATTICE_NODE_T)
#define TYPEDEF_TETENGO_LATTICE_NODE_T
typedef struct tetengo_lattice_node_tag tetengo_lattice_node_t;
#endif
#endif


/*!
    \brief Creates an empty constraint.

    It matches any path.

    \return A pointer to an empty constraint.
*/
tetengo_lattice_constraint_t* tetengo_lattice_constraint_createEmpty();

/*!
    \brief Creates a constraint.

    The ownership of the constraint elements pointed by pp_pattern is transferred into the constraint.
    There is no need to destroy the constraint elements after calling this function.
    But the array holding the constraint elements must be destroyed outside this function.

    \param pp_pattern     A pointer to an array of pointers to constraint elements.
    \param pattern_length A pattern length.

    \return A pointer to constraint. Or NULL when pp_pattern is NULL or has NULL constraint elements.
*/
tetengo_lattice_constraint_t*
tetengo_lattice_constraint_create(tetengo_lattice_constraintElement_t* const* pp_pattern, size_t pattern_length);

/*!
    \brief Destroys a constraint.

    \param p_constraint A pointer to a constraint.
*/
void tetengo_lattice_constraint_destroy(const tetengo_lattice_constraint_t* p_constraint);

/*!
    \brief Returns non-zero when the path matches the pattern.

    \param p_constraint   A pointer to a constraint.
    \param p_reverse_path A pointer to a path in reverse order.
    \param path_length    A path length.

    \retval non-zero When the path matches the pattern.
    \retval 0        Otherwise.
*/
int tetengo_lattice_constraint_matches(
    const tetengo_lattice_constraint_t* p_constraint,
    const tetengo_lattice_node_t*       p_reverse_path,
    size_t                              path_length);

/*!
    \brief Returns non-zero when the tail path matches the tail of the pattern.

    \param p_constraint        A pointer to a constraint.
    \param p_reverse_tail_path A poiner to a tail path in reverse order.
    \param tail_path_length    A tail path length.

    \retval non-zero When the tail path matches the tail of the pattern.
    \retval 0        Otherwise.
*/
int tetengo_lattice_constraint_matchesTail(
    const tetengo_lattice_constraint_t* p_constraint,
    const tetengo_lattice_node_t*       p_reverse_tail_path,
    size_t                              tail_path_length);


#if defined(__cplusplus)
}
#endif


#endif
