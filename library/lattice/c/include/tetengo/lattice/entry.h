/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_LATTICE_ENTRY_H)
#define TETENGO_LATTICE_ENTRY_H

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
    tetengo_lattice_stringview key;

    //! A surface.
    tetengo_lattice_stringview surface;

    //! A cost.
    int cost;


} tetengo_lattice_entry;


#if defined(__cplusplus)
}
#endif


#endif
