/*! \file
    \brief A JSON-to-timetabe.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "loadTimetable.h"
#include "printTimetable.h"
#include "timetable.h"


int main()
{
    timetable_t* const p_timetable = load_timetable();

    print_line_timetable(p_timetable);

    timetable_destroy(p_timetable);

    return 0;
}
