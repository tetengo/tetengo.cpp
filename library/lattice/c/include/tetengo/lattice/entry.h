/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_LATTICE_ENTRY_H)
#define TETENGO_LATTICE_ENTRY_H

#include <stddef.h>

#include <tetengo/lattice/string_view.h>


#if defined(__cplusplus)
extern "C" {
#endif

/*!
    \brief An entry.
*/
typedef struct tetengo_lattice_entry_tag
{
    //! A key.
    tetengo_lattice_stringView key;

    //! A surface.
    tetengo_lattice_stringView surface;

    //! A cost.
    int cost;


} tetengo_lattice_entry;

/*!
    \brief An entry map element.
*/
typedef struct tetengo_lattice_entry_map_element_tag
{
    //! A key.
    tetengo_lattice_stringView key;

    //! Entries
    const tetengo_lattice_entry* p_entries;

    //! An entry count.
    size_t entry_count;


} tetengo_lattice_entry_map_element;


#if defined(__cplusplus)
}
#endif


#endif
