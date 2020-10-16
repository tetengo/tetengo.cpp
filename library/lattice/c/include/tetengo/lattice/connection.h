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
#if !defined(TYPEDEF_TETENGO_LATTICE_ENTRY_T)
#define TYPEDEF_TETENGO_LATTICE_ENTRY_T
typedef struct tetengo_lattice_entry_tag tetengo_lattice_entry_t;
#endif
#endif


/*!
    \brief A connection.
*/
typedef struct tetengo_lattice_connection_tag
{
    //! A cost.
    int cost;


} tetengo_lattice_connection_t;

/*!
    \brief A pair of entries and a connection cost.
*/
typedef struct tetengo_lattice_entriesConnectionCostPair_tag
{
    //! A pointer to an origin entry.
    const tetengo_lattice_entry_t* p_from;

    //! A pointer to a destination entry.
    const tetengo_lattice_entry_t* p_to;

    //! A cost.
    int cost;


} tetengo_lattice_entriesConnectionCostPair_t;


#if defined(__cplusplus)
}
#endif


#endif
