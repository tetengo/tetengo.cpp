/*! \file
    \brief An entry.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_ENTRY_H)
#define TETENGO_LATTICE_ENTRY_H

#include <stddef.h>

#include <tetengo/lattice/stringView.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(TYPEDEF_TETENGO_LATTICE_ENTRY_VALUEHANDLE_T)
#define TYPEDEF_TETENGO_LATTICE_ENTRY_VALUEHANDLE_T
/*!
    \brief An entry value handle.
*/
typedef const struct tetengo_lattice_entry_valueHandle_tag* tetengo_lattice_entry_valueHandle_t;
#endif


/*!
    \brief An entry.
*/
typedef struct tetengo_lattice_entry_tag
{
    /*! A key. */
    tetengo_lattice_stringView_t key;

    /*! A pointer to a value. */
    const void* p_value;

    /*! A cost. */
    int cost;


} tetengo_lattice_entry_t;

/*!
    \brief An entry view.
*/
typedef struct tetengo_lattice_entryView_tag
{
    /*! A key. */
    tetengo_lattice_stringView_t key;

    /*! A value handle. */
    tetengo_lattice_entry_valueHandle_t value_handle;

    /*! A cost. */
    int cost;


} tetengo_lattice_entryView_t;

/*!
    \brief A pair of a key and entries.
*/
typedef struct tetengo_lattice_keyEntriesPair_tag
{
    /*! A key. */
    tetengo_lattice_stringView_t key;

    /*! Entries */
    const tetengo_lattice_entry_t* p_entries;

    /*! An entry count. */
    size_t entry_count;


} tetengo_lattice_keyEntriesPair_t;

/*!
    \brief Returns the pointer to the BOS/EOS (Beginning/End of Sequence) entry.

    \return The pointer to the BOS/EOS entry.
*/
const tetengo_lattice_entryView_t* tetengo_lattice_entry_bosEos();

/*!
    \brief Return the entry value by a handle.

    \param handle A handle.

    \return The value.
*/
const void* tetengo_lattice_entry_valueOf(tetengo_lattice_entry_valueHandle_t handle);


#if defined(__cplusplus)
}
#endif


#endif
