/*! \file
    \brief A JSON-to-timetabe.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <stdio.h>

#include "loadTimetable.h"
#include "printLineTimetable.h"
#include "timetable.h"


int main(const int argc, char** const argv)
{
    if (argc <= 1)
    {
        fprintf(stderr, "Usage: ../json2timetable timetable.json [station_name]\n");
        return 0;
    }

    {
        timetable_t* const p_timetable = load_timetable(argv[1]);
        if (p_timetable)
        {
            size_t station_index = (size_t)-1;
            if (argc > 2)
            {
                station_index = timetable_findStation(p_timetable, argv[2]);
            }
            if (station_index != (size_t)-1)
            {
                print_station_timetable(p_timetable, station_index);
            }
            else
            {
                print_line_timetable(p_timetable);
            }
            timetable_destroy(p_timetable);
        }
    }

    return 0;
}
