/*! \file
    \brief An array list.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(ARRAYLIST_H)
#define ARRAYLIST_H


/*! The array list type. */
typedef struct arrayList_tag arrayList_t;

/*!
    \brief Creates an array list.

    \return A pointer to an array list.
*/
arrayList_t* arrayList_create();

/*!
    \brief Destroys an array list.

    \param p_array_list A pointer to an array list.
*/
void arrayList_destroy(const arrayList_t* p_array_list);


#endif
