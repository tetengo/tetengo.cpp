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
    \brief A custom input definition.
*/
typedef struct tetengo_lattice_customInputDefinition_tag
{
    /*! The pointer to the context. */
    void* p_context;

    /*!
        \brief The procedure for length.

        \param p_context A pointer to the context.

        \return The length. Or (size_t)-1 when p_input is NULL.
    */
    size_t (*length_proc)(void* p_context);

    /*!
        \brief The procedure for creating a subrange context.

        \param p_context A pointer to a superrange context.
        \param offset    An offset.
        \param length    A length.

        \return A pointer to a subrange context. Or offset and/or length are out of the range of the input.
    */
    void* (*create_subrange_context_proc)(void* p_context, size_t offset, size_t length);

    /*!
        \brief The procedure for destroying a subrange context.

        \param p_context A pointer to a subrange context.
    */
    void (*destroy_subraneg_context_proc)(void* p_context);

    /*!
        \brief The procedure for append.

        The ownership of the input pointed by p_another is transferred into this input.
        There is no need to destroy the input p_another after calling this function.

        \param p_context A pointer to the context.
        \param p_another A pointer to another input.

        \retval non-zero When p_another is appended to this input.
        \retval 0        Otherwise.
    */
    int (*append_proc)(void* p_context, tetengo_lattice_input_t* p_another);

} tetengo_lattice_customInputDefinition_t;

/*!
    \brief Creates a custom input.

    \param p_definition A pointer to a definition.

    \return A pointer to a custom input. Or NULL when p_definition NULL
*/
tetengo_lattice_input_t*
tetengo_lattice_input_createCustomInput(const tetengo_lattice_customInputDefinition_t* p_definition);

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
    \brief Creates a subrange.

    \param p_input A pointer to an input.
    \param offset  An offset.
    \param length  A length.

    \return A pointer to a subrange.
            Or NULL when p_input is NULL, or offset and/or length are out of the range of the input.
*/
tetengo_lattice_input_t*
tetengo_lattice_input_createSubrange(const tetengo_lattice_input_t* p_input, size_t offset, size_t length);

/*!
    \brief Appends another input.

    The ownership of the input pointed by p_another is transferred into this input.
    There is no need to destroy the input p_another after calling this function.

    \param p_input   A pointer to an input.
    \param p_another A pointer to another input.

    \retval non-zero When p_another is appended to this input.
    \retval 0        Otherwise.
*/
int tetengo_lattice_input_append(tetengo_lattice_input_t* p_input, tetengo_lattice_input_t* p_another);

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

/*!
    \brief Returns the context of a custom input.

    \param p_custom_input A pointer to a custom input.

    \return A pointer to the context. Or NULL when p_custom_input is NULL or is not a custom input.
*/
const void* tetengo_lattice_customInput_context(const tetengo_lattice_input_t* p_custom_input);

/*!
    \brief Sets a context of a custom input.

    \param p_custom_input A pointer to a custom input.
    \param p_context      A pointer to a context.

    \retval non-zero When the context is set.
    \retval 0        Otherwise.
*/
int tetengo_lattice_customInput_setContext(tetengo_lattice_input_t* p_custom_input, void* p_context);


#if defined(__cplusplus)
}
#endif


#endif
