/*! \file
    \brief A property set.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_PROPERTYSET_H)
#define TETENGO_PROPERTY_PROPERTYSET_H

#include <stddef.h>
#include <stdint.h> // IWYU pragma: keep
#if defined(IWYU)
#include <cstdint> // Just silencing IWYU
#endif

#include <tetengo/property/storage.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_property_propertySet_tag tetengo_property_propertySet_t;
#endif


/*!
    \brief Creates a property set.

    The ownership of the storage loader pointed by p_storage_loader is transferred into the property set.
    There is no need to destroy the storage loader after calling this function.

    \param p_storage_loader A pointer to a storage loader.
    \param path             A path.

    \return A pointer to a property set. Or NULL on error.
*/
tetengo_property_propertySet_t*
tetengo_property_propertySet_create(tetengo_property_storageLoader_t* p_storage_loader, const char* path);

/*!
    \brief Destroys a property set.

    \param p_property_set A pointer to a property set.
*/
void tetengo_property_propertySet_destroy(const tetengo_property_propertySet_t* p_property_set);

/*!
    \brief Returns the value in a boolean.

    \param p_property_set A pointer to a property set.
    \param key            A key.
    \param p_value        The storage for a value.

    \retval non-zero When the value is set to p_value.
    \retval 0        Otherwise.
*/
int tetengo_property_propertySet_getBool(
    const tetengo_property_propertySet_t* p_property_set,
    const char*                           key,
    int*                                  p_value);

/*!
    \brief Sets a value in a boolean.

    \param p_property_set A pointer to a property set.
    \param key            A key.
    \param value          A value.
*/
void tetengo_property_propertySet_setBool(tetengo_property_propertySet_t* p_property_set, const char* key, int value);

/*!
    \brief Returns the value in an unsigned 32-bit integer.

    \param p_property_set A pointer to a property set.
    \param key            A key.
    \param p_value        The storage for a value.

    \retval non-zero When the value is set to p_value.
    \retval 0        Otherwise.
*/
int tetengo_property_propertySet_getUint32(
    const tetengo_property_propertySet_t* p_property_set,
    const char*                           key,
    uint32_t*                             p_value);

/*!
    \brief Sets a value in an unsigned 32-bit integer.

    \param p_property_set A pointer to a property set.
    \param key            A key.
    \param value          A value.
*/
void tetengo_property_propertySet_setUint32(
    tetengo_property_propertySet_t* p_property_set,
    const char*                     key,
    uint32_t                        value);

/*!
    \brief Returns the value in a string.

    \param p_property_set A pointer to a property set.
    \param key            A key.
    \param p_value        The storage for a value. Can be NULL.
    \param value_capacity A value capacity.

    \return The value length. Or (size_t)-1 when no value for key is found.
*/
size_t tetengo_property_propertySet_getString(
    const tetengo_property_propertySet_t* p_property_set,
    const char*                           key,
    char*                                 p_value,
    size_t                                value_capacity);

/*!
    \brief Sets a value in a string.

    \param p_property_set A pointer to a property set.
    \param key            A key.
    \param value          A value.
*/
void tetengo_property_propertySet_setString(
    tetengo_property_propertySet_t* p_property_set,
    const char*                     key,
    const char*                     value);

/*!
    \brief Updates the property values to the latest state in the storage.

    \param p_property_set A pointer to a property set.
*/
void tetengo_property_propertySet_update(tetengo_property_propertySet_t* p_property_set);

/*!
    \brief Commits the property value changes to the storage.

    \param p_property_set A pointer to a property set.
*/
void tetengo_property_propertySet_commit(const tetengo_property_propertySet_t* p_property_set);


#if defined(__cplusplus)
}
#endif


#endif
