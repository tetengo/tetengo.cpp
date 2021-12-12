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

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_entry_tag tetengo_lattice_entry_t;
#endif


/*!
    \brief An entry value handle.
*/
typedef const struct tetengo_lattice_entry_valueHandle_tag* tetengo_lattice_entry_valueHandle_t;

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
    const tetengo_lattice_entry_t** p_entries;

    /*! An entry count. */
    size_t entry_count;


} tetengo_lattice_keyEntriesPair_t;

/*!
    \brief Returns the pointer to the BOS/EOS (Beginning/End of Sequence) entry.

    \return The pointer to the BOS/EOS entry.
*/
const tetengo_lattice_entryView_t* tetengo_lattice_entry_bosEos();

/*!
    \brief Creates an entry.

    \param p_key   A pointer to a key.
    \param p_value A pointer to a value.
    \param cost    A cost.

    \return A pointer to an entry. Or NULL when p_key is NULL.
*/
tetengo_lattice_entry_t*
tetengo_lattice_entry_create(const tetengo_lattice_stringView_t* p_key, const void* p_value, int cost);

/*!
    \brief Destroys an entry.

    \param p_entry A pointer to an entry.
*/
void tetengo_lattice_entry_destroy(const tetengo_lattice_entry_t* p_entry);

/*!
    \brief Returns the key.

    \param p_entry A pointer to an entry.

    \return The key. Or NULL when p_entry is NULL.
*/
const tetengo_lattice_stringView_t* tetengo_lattice_entry_key(const tetengo_lattice_entry_t* p_entry);

/*!
    \brief Returns the value.

    \param p_entry A pointer to an entry.

    \return The value. Or NULL when p_entry is NULL.
*/
const void* tetengo_lattice_entry_value(const tetengo_lattice_entry_t* p_entry);

/*!
    \brief Returns the cost.

    \param p_entry A pointer to an entry.

    \return The cost. Or INT_MAX when p_entry is NULL.
*/
int tetengo_lattice_entry_cost(const tetengo_lattice_entry_t* p_entry);

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
