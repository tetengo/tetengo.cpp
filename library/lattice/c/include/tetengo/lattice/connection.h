/*! \file
    \brief A connection.

    $12026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_CONNECTION_H)
#define TETENGO_LATTICE_CONNECTION_H

#include <tetengo/lattice/entry.h>


#if defined(__cplusplus)
extern "C" {
#endif


/*!
    \brief A connection.
*/
typedef struct tetengo_lattice_connection_tag
{
    /*! A cost. */
    int cost;


} tetengo_lattice_connection_t;

/*!
    \brief A pair of entries and a connection cost.
*/
typedef struct tetengo_lattice_entriesConnectionCostPair_tag
{
    /*! A pointer to an origin entry. */
    const tetengo_lattice_entry_t* p_from;

    /*! A pointer to a destination entry. */
    const tetengo_lattice_entry_t* p_to;

    /*! A cost. */
    int cost;


} tetengo_lattice_entriesConnectionCostPair_t;


#if defined(__cplusplus)
}
#endif


#endif
