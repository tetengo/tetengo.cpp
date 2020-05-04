/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_LATTICE_LATTICE_H)
#define TETENGO_LATTICE_LATTICE_H


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_lattice_tag    tetengo_lattice_lattice_t;
typedef struct tetengo_lattice_vocabulary_tag tetengo_lattice_vocabulary_t;
#endif


/*!
    \brief Creates a lattice.

    p_vocabulary is destroyed by this lattice.

    \param p_vocabulary A pointer to a vocabulary.

    \return A pointer to a lattice. Or NULL when p_vocabulary is NULL.
*/
tetengo_lattice_lattice_t* tetengo_lattice_lattice_create(tetengo_lattice_vocabulary_t* p_vocabulary);

/*!
    \brief Destroys the lattice.

    \param p_lattice A pointer to a lattice.
*/
void tetengo_lattice_lattice_destroy(const tetengo_lattice_lattice_t* p_lattice);


#if defined(__cplusplus)
}
#endif


#endif
