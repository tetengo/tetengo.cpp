/*! \file
    \brief A timetable.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "timetable.h"

#include <stdlib.h>
#include <string.h>

#include "arrayList.h"


struct timetable_tag
{
    arrayList_t* p_stations;
};

static char* duplicate_string(const char* const string)
{
    const size_t length = strlen(string);
    char* const  duplication = malloc((length + 1) * sizeof(char));
    if (!duplication)
    {
        return NULL;
    }
    strcpy(duplication, string);
    duplication[length] = '\0';
    return duplication;
}

static void destroy_element(const void* const p_element)
{
    free((char*)p_element);
}

timetable_t* timetable_create(const char* const* const p_stations, const size_t station_count)
{
    if (!p_stations)
    {
        return NULL;
    }
    if (station_count == 0)
    {
        return NULL;
    }

    {
        timetable_t* const p_timetable = malloc(sizeof(timetable_t));
        if (!p_timetable)
        {
            return NULL;
        }
        p_timetable->p_stations = arrayList_create(destroy_element);

        {
            size_t i = 0;
            for (i = 0; i < station_count; ++i)
            {
                arrayList_add(p_timetable->p_stations, duplicate_string(p_stations[i]));
            }
        }

        return p_timetable;
    }
}

void timetable_destroy(const timetable_t* const p_timetable)
{
    arrayList_destroy(p_timetable->p_stations);
    free((void*)p_timetable);
}
