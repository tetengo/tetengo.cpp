/*! \file
    \brief An array list.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#if !defined(ARRAYLIST_H)
#define ARRAYLIST_H

#include <stddef.h>


/*! The array list type. */
typedef struct arrayList_tag arrayList_t;

/*!
    \brief Destroys an element.

    \param p_element A pointer to an element.
*/
typedef void (*destroy_element_t)(const void* p_element);

/*!
    \brief Creates an array list.

    \param p_destroy_element A pointer to a function to destroy an element.

    \return A pointer to an array list. Or NULL on error.
*/
arrayList_t* arrayList_create(destroy_element_t p_destroy_element);

/*!
    \brief Destroys an array list.

    \param p_array_list A pointer to an array list.
*/
void arrayList_destroy(const arrayList_t* p_array_list);

/*!
    \brief Adds an element.

    \param p_array_list A pointer to an array list.
    \param p_element    A pointer to an element.
*/
void arrayList_add(arrayList_t* p_array_list, void* p_element);

/*!
    \brief Returns the size.

    \param p_array_list A pointer to an array list.

    \return The size. Or 0 when p_array_list is NULL.
*/
size_t arrayList_size(const arrayList_t* p_array_list);

/*!
    \brief Returns the element.

    \param p_array_list A pointer to an array list.
    \param index        An index.

    \return A pointer to the element. Or NULL when p_array_list is NULL or index is greater than the size.
*/
const void* arrayList_at(const arrayList_t* p_array_list, size_t index);

/*!
    \brief Returns the mutable element.

    \param p_array_list A pointer to an array list.
    \param index        An index.

    \return A pointer to the mutable element. Or NULL when p_array_list is NULL or index is greater than the size.
*/
void* arrayList_mutableAt(arrayList_t* p_array_list, size_t index);

/*!
    \brief Returns a pointer to the elements.

    \param p_array_list A pointer to an array list.

    \return A pointer to the elements.
*/
const void* const* arrayList_data(const arrayList_t* p_array_list);


#endif
