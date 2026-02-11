/*! \file
    \brief An element.

    $12026 kaoru  https://www.tetengo.org/
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

/*! The type type. */
typedef struct tetengo_json_element_type_tag
{
    /*! A name. */
    int name;

    /*! A category. */
    int category;
} tetengo_json_element_type_t;

/*! The attribute key-value type. */
typedef struct tetengo_json_element_attributeKeyValue_tag
{
    /*! A key. */
    const char* key;

    /*! A value. */
    const char* value;
} tetengo_json_element_attributeKeyValue_t;

/*! The file location type. */
typedef struct tetengo_json_fileLocation_tag
{
    /*! A line. */
    const char* line;

    /*! A line index. */
    size_t line_index;

    /*! A column index. */
    size_t column_index;
} tetengo_json_fileLocation_t;

/*!
    \brief Creates an element.

    \param p_type          A pointer to a type.
    \param value           A value.
    \param p_attributes    A pointer to the head of attributes.
    \param attribute_count An attribute count.
    \param p_file_location A pointer to a file location.

    \return A pointer to an element. Or NULL on error.
*/
tetengo_json_element_t* tetengo_json_element_create(
    const tetengo_json_element_type_t*              p_type,
    const char*                                     value,
    const tetengo_json_element_attributeKeyValue_t* p_attributes,
    size_t                                          attribute_count,
    const tetengo_json_fileLocation_t*              p_file_location);

/*!
    \brief Copies an element.

    \param p_another A pointer to another element.

    \return A pointer to an element. Or NULL when p_another is NULL.
*/
tetengo_json_element_t* tetengo_json_element_copy(const tetengo_json_element_t* p_another);

/*!
    \brief Destroys an element.

    \param p_element A pointer to an element.
*/
void tetengo_json_element_destroy(const tetengo_json_element_t* p_element);

/*!
    \brief Returns the type.

    \param p_element A pointer to an element.

    \return The pointer to the type. Or NULL when p_element is NULL.
*/
const tetengo_json_element_type_t* tetengo_json_element_type(const tetengo_json_element_t* p_element);

/*!
    \brief Returns the value.

    \param p_element A pointer to an element.

    \return The value. Or NULL when p_element is NULL.
*/
const char* tetengo_json_element_value(const tetengo_json_element_t* p_element);

/*!
    \brief Returns the attribute keys.

    \param p_element A pointer to an element.
    \param p_keys    The storage for a keys. Can be NULL.

    \return The attribute count (== The key count). Or 0 when p_element is NULL.
*/
size_t tetengo_json_element_attributeKeys(const tetengo_json_element_t* p_element, const char** p_keys);

/*!
    \brief Returns the attribute value.

    \param p_element A pointer to an element.
    \param key       A key.

    \return The value. Or NULL when p_element is NULL or key is NULL or key is not found.
*/
const char* tetengo_json_element_attributeValueOf(const tetengo_json_element_t* p_element, const char* key);

/*!
    \brief Returns the file location.

    \param p_element A pointer to an element.

    \return A pointer to the file location. Or NULL when p_element is NULL.
*/
const tetengo_json_fileLocation_t* tetengo_json_element_getFileLocation(const tetengo_json_element_t* p_element);


#if defined(__cplusplus)
}
#endif


#endif
