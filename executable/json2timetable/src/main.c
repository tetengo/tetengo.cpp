/*! \file
    \brief A JSON-to-timetabe.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "arrayList.h"


int main()
{
    arrayList_t* const p_array_list = arrayList_create();

    arrayList_destroy(p_array_list);

    return 0;
}
