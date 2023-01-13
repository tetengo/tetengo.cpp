/*! \file
    \brief A path.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_PATH_H)
#define TETENGO_LATTICE_PATH_H

#include <stdbool.h>
#include <stddef.h>

#include <tetengo/lattice/node.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_path_tag tetengo_lattice_path_t;
#endif


/*!
    \brief Creates an empty path.

    \return A pointer to a path. Or NULL when p_nodes is NULL.
*/
tetengo_lattice_path_t* tetengo_lattice_path_createEmpty();

/*!
    \brief Creates a path.

    \param p_nodes    A pointer to nodes.
    \param node_count A node count.
    \param cost       A cost.

    \return A pointer to a path. Or NULL when p_nodes is NULL and noce_count is greater than 0.
*/
tetengo_lattice_path_t* tetengo_lattice_path_create(const tetengo_lattice_node_t* p_nodes, size_t node_count, int cost);

/*!
    \brief Destroys a path.

    \param p_path A pointer to a path.
*/
void tetengo_lattice_path_destroy(const tetengo_lattice_path_t* p_path);

/*!
    \brief Returns true when this path is empty.

    \param p_path A pointer to a path.

    \retval true  When this path is empty.
    \retval false Otherwise.
*/
bool tetengo_lattice_path_empty(const tetengo_lattice_path_t* p_path);

/*!
    \brief Returns the nodes.

    The second and the latter nodes follows the head node continuously.

    \param p_path  A pointer to a path.
    \param p_nodes The storage for the nodes. Can be NULL.

    \return The node count. Or 0 when p_path is NULL.
*/
size_t tetengo_lattice_path_pNodes(const tetengo_lattice_path_t* p_path, tetengo_lattice_node_t* p_nodes);

/*!
    \brief Returns the cost.

    \param p_path A pointer to a path.

    \return The cost. Or 0 when p_path is NULL.
*/
int tetengo_lattice_path_cost(const tetengo_lattice_path_t* p_path);


#if defined(__cplusplus)
}
#endif


#endif
