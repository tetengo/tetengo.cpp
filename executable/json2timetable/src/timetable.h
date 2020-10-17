/*! \file
    \brief A timetable.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TIMETABLE_H)
#define TIMETABLE_H


/*! The timetable type. */
typedef struct timetable_tag timetable_t;

/*!
    \brief Creates a timetable.

    \return A pointer to a timetable. Or NULL on error.
*/
timetable_t* timetable_create();

/*!
    \brief Destroys a timetable.

    \param p_timetable A pointer to a timetable.
*/
void timetable_destroy(const timetable_t* p_timetable);


#endif
