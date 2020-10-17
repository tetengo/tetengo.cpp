/*! \file
    \brief A JSON-to-timetabe.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <stdio.h>
#include <stdlib.h>

#include "arrayList.h"


static void destroy_element(const void* const p_element)
{
    free((void*)p_element);
}

int main()
{
    arrayList_t* const p_array_list = arrayList_create(destroy_element);
    {
        int i = 0;
        for (i = 0; i < 10; ++i)
        {
            int* const p_integer = malloc(sizeof(int));
            if (p_integer)
            {
                *p_integer = i * 2;
                arrayList_add(p_array_list, p_integer);
            }
        }
    }
    {
        size_t i = 0;
        for (i = 0; i < arrayList_size(p_array_list); ++i)
        {
            const int* const p_integer = (const int*)arrayList_at(p_array_list, i);
            if (p_integer)
            {
                printf("%d, ", *p_integer);
            }
        }
        printf("\n");
    }
    arrayList_destroy(p_array_list);

    return 0;
}
