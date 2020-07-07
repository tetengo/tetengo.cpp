/*! \file
    \brief A path.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_LATTICE_PATH_H)
#define TETENGO_LATTICE_PATH_H

#include <stddef.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_path_tag tetengo_lattice_path_t;
typedef struct tetengo_lattice_node_tag tetengo_lattice_node_t;
#endif


/*!
    \brief Creates a path.

    \param p_nodes    A pointer to nodes.
    \param node_count A node count.
    \param cost       A cost.

    \return A pointer to a path. Or NULL when p_nodes is NULL.
*/
tetengo_lattice_path_t*
tetengo_lattice_path_create(const tetengo_lattice_node_t* p_nodes, size_t node_count, size_t cost);

/*!
    \brief Destroys a path.

    \param p_path A pointer to a path.
*/
void tetengo_lattice_path_destroy(const tetengo_lattice_path_t* p_path);

/*!
    \brief Returns the nodes.

    The second and the latter nodes follows the head node continuously.

    \param p_path  A pointer to a path.
    \param p_nodes The storage for the nodes. Can be NULL.

    \return The node count. Or 0 when p_path is NULL.
*/
size_t tetengo_lattice_path_pNodes(const tetengo_lattice_path_t* p_path, tetengo_lattice_node_t* p_nodes);


#if defined(__cplusplus)
}
#endif


#endif
