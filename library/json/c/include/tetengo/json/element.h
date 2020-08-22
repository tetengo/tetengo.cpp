/*! \file
    \brief An element.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_JSON_ELEMENT_H)
#define TETENGO_JSON_ELEMENT_H

#include <stddef.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_json_element_tag tetengo_json_element_t;
#endif


/*!
    \brief Returns the type name of string.

    \return The type name.
*/
int tetengo_json_element_typeName_string();

/*!
    \brief Returns the type name of number.

    \return The type name.
*/
int tetengo_json_element_typeName_number();

/*!
    \brief Returns the type name of boolean.

    \return The type name.
*/
int tetengo_json_element_typeName_boolean();

/*!
    \brief Returns the type name of null.

    \return The type name.
*/
int tetengo_json_element_typeName_null();

/*!
    \brief Returns the type name of object.

    \return The type name.
*/
int tetengo_json_element_typeName_object();

/*!
    \brief Returns the type name of member.

    \return The type name.
*/
int tetengo_json_element_typeName_member();

/*!
    \brief Returns the type name of array.

    \return The type name.
*/
int tetengo_json_element_typeName_array();

/*!
    \brief Returns the type category of primitive.

    \return The type category.
*/
int tetengo_json_element_typeCategory_primitive();

/*!
    \brief Returns the type category of opening structure.

    \return The type category.
*/
int tetengo_json_element_typeCategory_structureOpen();

/*!
    \brief Returns the type category of closing structure.

    \return The type category.
*/
int tetengo_json_element_typeCategory_structureClose();

//! The type type.
struct tetengo_json_element_type_t
{
    //! A name.
    int name;

    //! A category.
    int category;
};

//! The attribute key-value type.
struct tetengo_json_element_attributeKeyValue_t
{
    //! A key.
    const char* key;

    //! A value.
    const char* value;
};

/*!
    \brief Creates an element.

    \param p_type          A pointer to a type.
    \param value           A value.
    \param p_attributes    A pointer to the head of attributes.
    \param attribute_count An attribute count.

    \return A pointer to an element. Or NULL on error.
*/
tetengo_json_element_t* tetengo_json_element_create(
    const tetengo_json_element_type_t*              p_type,
    const char*                                     value,
    const tetengo_json_element_attributeKeyValue_t* p_attributes,
    size_t                                          attribute_count);

/*!
    \brief Destroys an element.

    \param p_element A pointer to an element.
*/
void tetengo_json_element_destroy(const tetengo_json_element_t* p_element);


#if defined(__cplusplus)
}
#endif


#endif
