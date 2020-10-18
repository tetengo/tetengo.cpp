/*! \file
    \brief A JSON-to-timetabe.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "timetable.h"


static timetable_t* load_timetable()
{
    const char* p_stations[] = { "Hoge", "Fuga", "Piyo" };
    return timetable_create(p_stations, sizeof(p_stations) / sizeof(const char*));
}

int main()
{
    timetable_t* const p_timetable = load_timetable();

    timetable_destroy(p_timetable);

    return 0;
}
