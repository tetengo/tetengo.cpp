/*! \file
    \brief A station timetable printing.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#if !defined(PRINTSTATIONTIMETABLE_H)
#define PRINTSTATIONTIMETABLE_H

#include <stddef.h>

#include "timetable.h"


/*!
    \brief Prints a station timetable.

    \param p_timetable   A pointer to a timetable.
    \param station_index A station index.
*/
void print_station_timetable(const timetable_t* p_timetable, size_t station_index);


#endif
