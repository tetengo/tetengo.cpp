/*! \file
    \brief A connection.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_LATTICE_CONNECTION_H)
#define TETENGO_LATTICE_CONNECTION_H


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_entry_tag tetengo_lattice_entry;
#endif


/*!
    \brief A connection.
*/
typedef struct tetengo_lattice_connection_tag
{
    //! A cost.
    int cost;


} tetengo_lattice_connection;

/*!
    \brief A connection map element.
*/
typedef struct tetengo_lattice_connection_map_element_tag
{
    //! A pointer to an origin entry.
    const tetengo_lattice_entry* p_from;

    //! A pointer to a destination entry.
    const tetengo_lattice_entry* p_to;

    //! A cost.
    int cost;


} tetengo_lattice_connection_map_element;


#if defined(__cplusplus)
}
#endif


#endif
