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

/*!
    \brief An entry key handle.
*/
typedef const struct tetengo_lattice_entry_keyHandle_tag* tetengo_lattice_entry_keyHandle_t;

/*!
    \brief An entry view key handle.
*/
typedef const struct tetengo_lattice_entryView_keyHandle_tag* tetengo_lattice_entryView_keyHandle_t;

/*!
    \brief An entry view value handle.
*/
typedef const struct tetengo_lattice_entryView_valueHandle_tag* tetengo_lattice_entryView_valueHandle_t;


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
    tetengo_lattice_entryView_valueHandle_t value_handle;

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
    \brief Return the entry key by a handle.

    \param handle A handle.

    \return The key.
*/
tetengo_lattice_stringView_t tetengo_lattice_entry_keyOf(tetengo_lattice_entry_keyHandle_t handle);

/*!
    \brief Create an entry key.

    \param p_content A pointer to a content.

    \return The key handle. Or NULL when p_content is NULL.
*/
tetengo_lattice_entry_keyHandle_t tetengo_lattice_entry_createKey(const tetengo_lattice_stringView_t* p_content);

/*!
    \brief Destroy an entry key.

    \param handle A handle.
*/
void tetengo_lattice_entry_destroyKey(tetengo_lattice_entry_keyHandle_t handle);

/*!
    \brief Returns the pointer to the BOS/EOS (Beginning/End of Sequence) entry.

    \return The pointer to the BOS/EOS entry.
*/
const tetengo_lattice_entryView_t* tetengo_lattice_entryView_bosEos();

/*!
    \brief Return the entry view key by a handle.

    \param handle A handle.

    \return The key.
*/
tetengo_lattice_stringView_t tetengo_lattice_entryView_keyOf(tetengo_lattice_entryView_keyHandle_t handle);

/*!
    \brief Return the entry view value by a handle.

    \param handle A handle.

    \return The value.
*/
const void* tetengo_lattice_entryView_valueOf(tetengo_lattice_entryView_valueHandle_t handle);


#if defined(__cplusplus)
}
#endif


#endif
