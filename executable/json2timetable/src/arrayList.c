/*! \file
    \brief An array list.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "arrayList.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>


struct arrayList_tag
{
    size_t            capacity;
    size_t            size;
    void**            pp_elements;
    destroy_element_t p_destroy_element;
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
        void** const pp_new_elements = realloc(p_array_list->pp_elements, new_capacity * sizeof(void**));
        if (!pp_new_elements)
        {
            return;
        }
        p_array_list->capacity = new_capacity;
        p_array_list->pp_elements = pp_new_elements;
    }
}

arrayList_t* arrayList_create(const destroy_element_t p_destroy_element)
{
    arrayList_t* const p_array_list = malloc(sizeof(arrayList_t));
    if (!p_array_list)
    {
        return NULL;
    }
    p_array_list->capacity = 0;
    p_array_list->size = 0;
    p_array_list->pp_elements = NULL;
    p_array_list->p_destroy_element = p_destroy_element;

    reserve(p_array_list, 8);

    return p_array_list;
}

void arrayList_destroy(const arrayList_t* const p_array_list)
{
    if (!p_array_list)
    {
        return;
    }

    {
        size_t i = 0;
        for (i = 0; i < p_array_list->size; ++i)
        {
            p_array_list->p_destroy_element(p_array_list->pp_elements[i]);
        }
        free(p_array_list->pp_elements);
        free((void*)p_array_list);
    }
}

void arrayList_add(arrayList_t* const p_array_list, void* const p_element)
{
    if (!p_array_list)
    {
        return;
    }

    while (p_array_list->capacity < p_array_list->size + 1)
    {
        reserve(p_array_list, p_array_list->capacity * 2);
    }
    p_array_list->pp_elements[p_array_list->size] = p_element;
    ++p_array_list->size;
}

size_t arrayList_size(const arrayList_t* const p_array_list)
{
    if (!p_array_list)
    {
        return 0;
    }

    return p_array_list->size;
}

const void* arrayList_at(const arrayList_t* const p_array_list, const size_t index)
{
    if (!p_array_list)
    {
        return NULL;
    }
    if (index >= p_array_list->size)
    {
        return NULL;
    }

    return p_array_list->pp_elements[index];
}
