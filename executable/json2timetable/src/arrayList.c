/*! \file
    \brief An array list.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "arrayList.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>


typedef enum element_kind_tag
{
    element_kind_int,
    element_kind_string
} element_kind_t;

typedef struct element_tag
{
    element_kind_t kind;
    union
    {
        int         int_value;
        const char* string_value;
    } value;
} element_t;

struct arrayList_tag
{
    size_t     capacity;
    size_t     size;
    element_t* p_elements;
};

static void reserve(arrayList_t* p_array_list, const size_t new_capacity)
{
    if (!p_array_list)
    {
        return;
    }
    if (new_capacity <= p_array_list->capacity)
    {
        return;
    }
    assert(new_capacity > p_array_list->size);

    {
        element_t* const p_new_elements = realloc(p_array_list->p_elements, new_capacity * sizeof(element_t));
        if (!p_new_elements)
        {
            return;
        }
        p_array_list->capacity = new_capacity;
        p_array_list->p_elements = p_new_elements;
    }
}

arrayList_t* arrayList_create()
{
    arrayList_t* const p_array_list = malloc(sizeof(arrayList_t));
    if (!p_array_list)
    {
        return NULL;
    }

    p_array_list->capacity = 0;
    p_array_list->size = 0;
    p_array_list->p_elements = NULL;

    reserve(p_array_list, 8);

    return p_array_list;
}

void arrayList_destroy(const arrayList_t* const p_array_list)
{
    free(p_array_list->p_elements);
    free((void*)p_array_list);
}
