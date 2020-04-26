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

/*!
    \brief A node.
*/
typedef struct tetengo_lattice_node_tag
{
    //! A key.
    tetengo_lattice_stringView key;

    //! A pointer to a value.
    const void* p_value;

    //! An index of preceding nodes.
    size_t preceding;

    //! A node cost.
    int node_cost;

    //! A path cost.
    int path_cost;


} tetengo_lattice_node;


#if defined(__cplusplus)
}
#endif


#endif
