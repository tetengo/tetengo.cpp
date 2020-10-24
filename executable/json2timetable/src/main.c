/*! \file
    \brief A JSON-to-timetabe.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <stdio.h>

#include "loadTimetable.h"
#include "printTimetable.h"
#include "timetable.h"


int main(const int argc, char** const argv)
{
    if (argc <= 1)
    {
        fprintf(stderr, "Usage: ../json2timetable timetable.json\n");
        return 0;
    }

    {
        timetable_t* const p_timetable = load_timetable(argv[1]);
        print_line_timetable(p_timetable);
        timetable_destroy(p_timetable);
    }

    return 0;
}
