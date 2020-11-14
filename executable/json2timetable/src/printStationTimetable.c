/*! \file
    \brief A station timetable printing.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "printStationTimetable.h"

#include <stdio.h>
#include <stdlib.h>

#include "arrayList.h"
#include "timetable.h"


static void destroy_departure(const void* const p_departure)
{
    free((void*)p_departure);
}

static const arrayList_t* create_departure_list()
{
    arrayList_t* const p_departure_list = arrayList_create(destroy_departure);
    return p_departure_list;
}

static void destroy_departure_list(const void* const p_departure_list)
{
    arrayList_destroy(p_departure_list);
}

static const arrayList_t* create_departure_lists(const timetable_t* const p_timetable, const size_t station_index)
{
    arrayList_t* const p_departure_lists = arrayList_create(destroy_departure_list);
    (void)p_timetable;
    (void)station_index;
    {
        size_t i = 0;
        for (i = 0; i < 24; ++i)
        {
            arrayList_add(p_departure_lists, (void*)create_departure_list());
        }
    }
    return p_departure_lists;
}

void print_station_timetable(const timetable_t* const p_timetable, const size_t station_index)
{
    printf("%s\n", timetable_stationAt(p_timetable, station_index));
    {
        const arrayList_t* const p_departure_lists = create_departure_lists(p_timetable, station_index);

        arrayList_destroy(p_departure_lists);
    }
}
