/*! \file
    \brief A string view.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_STRINGVIEW_H)
#define TETENGO_LATTICE_STRINGVIEW_H

#include <stddef.h>


#if defined(__cplusplus)
extern "C" {
#endif

/*!
    \brief A string view.
*/
typedef struct tetengo_lattice_stringView_tag
{
    /*! A pointer to the head. */
    const char* p_head;

    /*! The length. */
    size_t length;


} tetengo_lattice_stringView_t;


#if defined(__cplusplus)
}
#endif


#endif
