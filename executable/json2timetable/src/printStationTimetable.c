/*! \file
    \brief A station timetable printing.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "printStationTimetable.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <tetengo/text/graphemeSplitter.h>

#include "arrayList.h"
#include "timetable.h"


static const char* create_departure(const size_t minute)
{
    char* const p_departure = malloc(3 * sizeof(char));
    if (!p_departure)
    {
        return NULL;
    }
    sprintf(p_departure, "%02u", (unsigned int)minute);
    return p_departure;
}

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
    {
        size_t i = 0;
        for (i = 0; i < 24; ++i)
        {
            arrayList_add(p_departure_lists, (void*)create_departure_list());
        }
    }
    {
        size_t i = 0;
        for (i = 0; i < timetable_trainCount(p_timetable); ++i)
        {
            const int departure = timetable_trainTimeAt(p_timetable, i, station_index);
            if (departure < 0)
            {
                continue;
            }
            {
                const size_t hour = departure / 100;
                const size_t minute = departure % 100;
                assert(hour < 24);
                assert(minute < 60);
                arrayList_add(arrayList_mutableAt(p_departure_lists, hour), (void*)create_departure(minute));
            }
        }
    }
    return p_departure_lists;
}

static size_t calculate_grapheme_width(const tetengo_text_grapheme_t* const p_graphemes, const size_t grapheme_count)
{
    size_t width = 0;
    {
        size_t i = 0;
        for (i = 0; i < grapheme_count; ++i)
        {
            width += p_graphemes[i].width;
        }
    }
    return width;
}

static size_t calculate_departure_width(const arrayList_t* const p_departure_list)
{
    size_t                                       width = 0;
    const tetengo_text_graphemeSplitter_t* const p_grapheme_splitter = tetengo_text_graphemeSplitter_create();
    {
        size_t i = 0;
        for (i = 0; i < arrayList_size(p_departure_list); ++i)
        {
            const char* const departure = arrayList_at(p_departure_list, i);
            const size_t grapheme_count = tetengo_text_graphemeSplitter_split(p_grapheme_splitter, departure, NULL);
            tetengo_text_grapheme_t* const p_graphemes = malloc(sizeof(tetengo_text_grapheme_t) * grapheme_count);
            tetengo_text_graphemeSplitter_split(p_grapheme_splitter, departure, p_graphemes);
            width += 1 + calculate_grapheme_width(p_graphemes, grapheme_count);
            free(p_graphemes);
        }
    }
    tetengo_text_graphemeSplitter_destroy(p_grapheme_splitter);
    return width;
}

static size_t calculate_max_departure_width(const arrayList_t* const p_departure_lists)
{
    size_t max_width = 0;
    {
        size_t i = 0;
        assert(arrayList_size(p_departure_lists) == 24);
        for (i = 0; i < 24; ++i)
        {
            const size_t width = calculate_departure_width(arrayList_at(p_departure_lists, i));
            if (width > max_width)
            {
                max_width = width;
            }
        }
    }
    return max_width;
}

static void print_title(const timetable_t* const p_timetable, const size_t station_index)
{
    printf("%s\n", timetable_stationAt(p_timetable, station_index));
}

static void print_horizontal_line(const size_t max_departure_width)
{
    printf("+--+");
    {
        size_t i = 0;
        for (i = 0; i < max_departure_width; ++i)
        {
            printf("-");
        }
        printf("-+");
    }
    printf("\n");
}

static void print_table(const arrayList_t* const p_departure_lists, const size_t max_departure_width)
{
    size_t i = 0;
    assert(arrayList_size(p_departure_lists) == 24);
    for (i = 0; i < 24; ++i)
    {
        const size_t hour = (i + 3) % 24;
        printf("|%2u|", (unsigned int)hour);
        {
            const arrayList_t* const p_departure_list = arrayList_at(p_departure_lists, hour);
            {
                size_t j = 0;
                for (j = 0; j < arrayList_size(p_departure_list); ++j)
                {
                    printf(" %s", (const char*)arrayList_at(p_departure_list, j));
                }
            }
            {
                const size_t departure_width = calculate_departure_width(p_departure_list);
                {
                    size_t j = 0;
                    for (j = 0; j < max_departure_width - departure_width; ++j)
                    {
                        printf(" ");
                    }
                }
                printf(" |");
            }
        }
        printf("\n");
    }
    (void)max_departure_width;
}

void print_station_timetable(const timetable_t* const p_timetable, const size_t station_index)
{
    print_title(p_timetable, station_index);
    {
        const arrayList_t* const p_departure_lists = create_departure_lists(p_timetable, station_index);
        const size_t             max_departure_width = calculate_max_departure_width(p_departure_lists);
        print_horizontal_line(max_departure_width);
        print_table(p_departure_lists, max_departure_width);
        print_horizontal_line(max_departure_width);
        arrayList_destroy(p_departure_lists);
    }
}
