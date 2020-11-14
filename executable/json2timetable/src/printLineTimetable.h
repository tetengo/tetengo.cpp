/*! \file
    \brief A line timetable printing.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(PRINTTIMETABLE_H)
#define PRINTTIMETABLE_H

#include <stddef.h>

#include "timetable.h"


/*!
    \brief Prints a line timetable.

    \param p_timetable A pointer to a timetable.
*/
void print_line_timetable(const timetable_t* p_timetable);

/*!
    \brief Prints a station timetable.

    \param p_timetable   A pointer to a timetable.
    \param station_index A station index.
*/
void print_station_timetable(const timetable_t* p_timetable, size_t station_index);


#endif
