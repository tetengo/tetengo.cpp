/*! \file
    \brief A JSON-to-timetabe.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "timetable.h"


static timetable_t* load_timetable()
{
    static const char* p_stations[] = { "Hoge", "Fuga", "Piyo" };
    timetable_t*       p_timetable = timetable_create(p_stations, sizeof(p_stations) / sizeof(const char*));

    {
        static const int times[] = { -1, 600, 730 };
        timetable_addTrain(p_timetable, "101M", times);
    }

    return p_timetable;
}

int main()
{
    timetable_t* const p_timetable = load_timetable();

    timetable_destroy(p_timetable);

    return 0;
}
