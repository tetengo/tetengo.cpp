/*! \file
    \brief A timetable.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "timetable.h"

#include <stdlib.h>
#include <string.h>

#include "arrayList.h"


typedef struct train_tag
{
    const char*  name;
    arrayList_t* p_times;
} train_t;

struct timetable_tag
{
    arrayList_t* p_stations;
    arrayList_t* p_trains;
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
    free((void*)p_element);
}

static void destroy_train(const void* const p_train_as_void)
{
    if (!p_train_as_void)
    {
        return;
    }

    {
        const train_t* const p_train = (const train_t*)p_train_as_void;
        destroy_element(p_train->name);
        arrayList_destroy(p_train->p_times);
    }
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
        p_timetable->p_trains = arrayList_create(destroy_train);
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

void timetable_addTrain(timetable_t* const p_timetable, const char* const name, const int* const p_times)
{
    train_t* const p_train = malloc(sizeof(train_t));
    if (!p_train)
    {
        return;
    }

    p_train->name = duplicate_string(name);
    p_train->p_times = arrayList_create(destroy_element);
    {
        size_t i = 0;
        for (i = 0; i < arrayList_size(p_timetable->p_stations); ++i)
        {
            int* const p_time = malloc(sizeof(int));
            if (!p_time)
            {
                continue;
            }
            *p_time = p_times[i];
            arrayList_add(p_train->p_times, p_time);
        }
    }

    arrayList_add(p_timetable->p_trains, p_train);
}

void timetable_destroy(const timetable_t* const p_timetable)
{
    arrayList_destroy(p_timetable->p_trains);
    arrayList_destroy(p_timetable->p_stations);
    free((void*)p_timetable);
}
