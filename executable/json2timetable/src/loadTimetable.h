/*! \file
    \brief A timetable loading.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(LOADTIMETABLE_H)
#define LOADTIMETABLE_H

#include "timetable.h"


/*!
    \brief Loads a timetable.

    \return A pointer to a timetable. Or NULL on error.
*/
timetable_t* load_timetable();


#endif
