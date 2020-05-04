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
typedef struct tetengo_lattice_node_tag       tetengo_lattice_node_t;
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

/*!
    \brief Pushes back an input.

    \param p_lattice A pointer to a lattice.
    \param input     An input.

    \retval non-zero When the input is pushed back.
    \retval 0        Otherwise.
*/
int tetengo_lattice_lattice_pushBack(tetengo_lattice_lattice_t* p_lattice, const char* input);

/*!
    \brief Settles this lattice.

    You can modify the lattice after settlement.
    Modification of the lattice after settlement invalidate the EOS node.

    \param p_lattice A pointer to a lattice.
    \param p_eos_node The storage for an output EOS node.

    \retval non-zero When an EOS node is stored.
    \retval 0        Otherwise.
*/
int tetengo_lattice_lattice_settle(tetengo_lattice_lattice_t* p_lattice, tetengo_lattice_node_t* p_eos_node);


#if defined(__cplusplus)
}
#endif


#endif
