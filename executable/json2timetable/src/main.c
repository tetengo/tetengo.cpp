/*! \file
    \brief A JSON-to-timetabe.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "printTimetable.h"
#include "timetable.h"


static timetable_t* load_timetable()
{
    static const char* p_stations[] = { "HogeHoge", "Fg", "Piyo" };
    timetable_t*       p_timetable = timetable_create(p_stations, sizeof(p_stations) / sizeof(const char*));

    {
        static const int times[] = { -1, 600, 730 };
        timetable_addTrain(p_timetable, "101M", times);
    }
    {
        static const int times[] = { 1000, 1200, 1330 };
        timetable_addTrain(p_timetable, "103M", times);
    }
    {
        static const int times[] = { 1600, -1, 1900 };
        timetable_addTrain(p_timetable, "205M", times);
    }
    {
        static const int times[] = { 2200, 0, -1 };
        timetable_addTrain(p_timetable, "107M", times);
    }

    return p_timetable;
}

int main()
{
    timetable_t* const p_timetable = load_timetable();

    print_line_timetable(p_timetable);

    timetable_destroy(p_timetable);

    return 0;
}
