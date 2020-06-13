/*! \file
    \brief A constraint.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_LATTICE_CONSTRAINT_H)
#define TETENGO_LATTICE_CONSTRAINT_H


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_constraint_tag        tetengo_lattice_constraint_t;
typedef struct tetengo_lattice_constraintElement_tag tetengo_lattice_constraintElement_t;
#endif


/*!
    \brief Creates an empty constraint.

    It matches any path.

    \return A pointer to an empty constraint.
*/
const tetengo_lattice_constraint_t* tetengo_lattice_constraint_createEmpty();

/*!
    \brief Destroys a constraint.

    \param p_constraint A pointer to a constraint.
*/
void tetengo_lattice_constraint_destroy(const tetengo_lattice_constraint_t* p_constraint);


#if defined(__cplusplus)
}
#endif


#endif
