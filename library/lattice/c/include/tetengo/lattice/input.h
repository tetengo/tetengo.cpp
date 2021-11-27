/*! \file
    \brief An input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_INPUT_H)
#define TETENGO_LATTICE_INPUT_H

#include <stddef.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_lattice_input_tag tetengo_lattice_input_t;
#endif


/*!
    \brief Creates a string input.

    \param value A value.

    \return A pointer to an string input. Or NULL when value is NULL.
*/
tetengo_lattice_input_t* tetengo_lattice_input_createStringInput(const char* value);

/*!
    \brief Destroys an input.

    \param p_input A pointer to an input.
*/
void tetengo_lattice_input_destroy(const tetengo_lattice_input_t* p_input);

/*!
    \brief Returns the length.

    \param p_input A pointer to an input.

    \return The length. Or (size_t)-1 when p_input is NULL.
*/
size_t tetengo_lattice_input_length(const tetengo_lattice_input_t* p_input);

/*!
    \brief Returns the value of a string input.

    \param p_string_input A pointer to a string input.

    \return The value. Or NULL when p_string_input is NULL or is not a string input.
*/
const char* tetengo_lattice_stringInput_value(const tetengo_lattice_input_t* p_string_input);

/*!
    \brief Sets a value of a string input.

    \param p_string_input A pointer to a string input.
    \param value          A value.

    \retval non-zero When the value is set.
    \retval 0        Otherwise.
*/
int tetengo_lattice_stringInput_setValue(tetengo_lattice_input_t* p_string_input, const char* value);


#if defined(__cplusplus)
}
#endif


#endif
