/*! \file
    \brief A timetable.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#if !defined(TIMETABLE_H)
#define TIMETABLE_H

#include <stdlib.h>

/*! The timetable type. */
typedef struct timetable_tag timetable_t;

/*!
    \brief Creates a timetable.

    \param title         A title.
    \param p_stations    A pointer to stations.
    \param station_count A station count.

    \return A pointer to a timetable.
            Or NULL when title is NULL, p_stations is NULL, station_count is 0 or on some other error.
*/
timetable_t* timetable_create(const char* title, const char* const* p_stations, size_t station_count);

/*!
    \brief Destroys a timetable.

    \param p_timetable A pointer to a timetable.
*/
void timetable_destroy(const timetable_t* p_timetable);

/*!
    \brief Returns the title.

    \param p_timetable A pointer to a timetable.

    \return The title. Or NULL when p_timetable is NULL.
*/
const char* timetable_title(const timetable_t* p_timetable);

/*!
    \brief Returns the station count.

    \param p_timetable A pointer to a timetable.

    \return The station count. Or 0 when p_timetable is NULL.
*/
size_t timetable_stationCount(const timetable_t* p_timetable);

/*!
    \brief Returns the station.

    \param p_timetable A pointer to a timetable.
    \param index       An index.

    \return The station. Or NULL when p_timetable is NULL or index is greater than the station count.
*/
const char* timetable_stationAt(const timetable_t* p_timetable, size_t index);

/*!
    \brief Finds the station with station_name.

    \param p_timetable  A pointer to a timetable.
    \param station_name A station name.

    \return The station index. Or (size_t)-1 when station_name is not found.
*/
size_t timetable_findStation(const timetable_t* p_timetable, const char* station_name);

/*!
    \brief Returns the train count.

    \param p_timetable A pointer to a timetable.

    \return The train count. Or 0 when p_timetable is NULL.
*/
size_t timetable_trainCount(const timetable_t* p_timetable);

/*!
    \brief Returns the train name.

    \param p_timetable A pointer to a timetable.
    \param index       An index.

    \return The train name. Or NULL when p_timetable is NULL or index is greater than the train count.
*/
const char* timetable_trainNameAt(const timetable_t* p_timetable, size_t index);

/*!
    \brief Returns the train time.

    \param p_timetable A pointer to a timetable.
    \param index       An index.

    \return The train time. Or a negative value when p_timetable is NULL, train_index is greater than the train count or
            station_index is greater than the station count.
*/
int timetable_trainTimeAt(const timetable_t* p_timetable, size_t train_index, size_t station_index);

/*!
    \brief Adds a train.

    For the station the train does not stop, set its time to a negative value.

    \param p_timetable A pointer to a timetable.
    \param name        A name.
    \param p_times     A pointer to times. The time count must be same as the station count.
*/
void timetable_addTrain(timetable_t* p_timetable, const char* name, const int* p_times);


#endif
