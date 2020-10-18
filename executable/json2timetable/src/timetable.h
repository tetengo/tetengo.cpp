/*! \file
    \brief A timetable.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TIMETABLE_H)
#define TIMETABLE_H

#include <stdlib.h>

/*! The timetable type. */
typedef struct timetable_tag timetable_t;

/*!
    \brief Creates a timetable.

    \param p_stations    A pointer to stations.
    \param station_count A station count.

    \return A pointer to a timetable. Or NULL when p_stations is NULL, station_count is 0 or on some other error.
*/
timetable_t* timetable_create(const char* const* p_stations, size_t station_count);

/*!
    \brief Destroys a timetable.

    \param p_timetable A pointer to a timetable.
*/
void timetable_destroy(const timetable_t* p_timetable);

/*!
    \brief Adds a train.

    For the station the train does not stop, set its time to a negative value.

    \param p_timetable A pointer to a timetable.
    \param name        A name.
    \param p_times     A pointer to times. The time count must be same as the station count.
*/
void timetable_addTrain(timetable_t* p_timetable, const char* name, const int* p_times);


#endif
