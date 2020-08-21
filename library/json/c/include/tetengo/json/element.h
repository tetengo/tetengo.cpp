/*! \file
    \brief An element.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_JSON_ELEMENT_H)
#define TETENGO_JSON_ELEMENT_H


#if defined(__cplusplus)
extern "C" {
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


#if defined(__cplusplus)
}
#endif


#endif
