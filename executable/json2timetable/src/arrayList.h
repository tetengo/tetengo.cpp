/*! \file
    \brief An array list.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(ARRAYLIST_H)
#define ARRAYLIST_H


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


#endif
