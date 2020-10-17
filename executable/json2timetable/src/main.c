/*! \file
    \brief A JSON-to-timetabe.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <stdlib.h>

#include "arrayList.h"


static void destroy_element(const void* const p_element)
{
    free((void*)p_element);
}

int main()
{
    arrayList_t* const p_array_list = arrayList_create(destroy_element);

    arrayList_destroy(p_array_list);

    return 0;
}
