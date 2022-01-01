/*! \file
    \brief A storage.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_STORAGE_H)
#define TETENGO_PROPERTY_STORAGE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_property_storage_tag       tetengo_property_storage_t;
typedef struct tetengo_property_storageLoader_tag tetengo_property_storageLoader_t;
#endif


/*!
    \brief Destroys a storage.

    \param p_storage A pointer to a storage.
*/
void tetengo_property_storage_destroy(const tetengo_property_storage_t* p_storage);

/*!
    \brief Returns the value in a boolean.

    \param p_storage A pointer to a storage.
    \param key       A key.
    \param p_value   The storage for a value.

    \retval true  When the value is set to p_value.
    \retval false Otherwise.
*/
bool tetengo_property_storage_getBool(const tetengo_property_storage_t* p_storage, const char* key, bool* p_value);

/*!
    \brief Sets a value in a boolean.

    \param p_storage A pointer to a storage.
    \param key       A key.
    \param value     A value.
*/
void tetengo_property_storage_setBool(tetengo_property_storage_t* p_storage, const char* key, bool value);

/*!
    \brief Returns the value in an unsigned 32-bit integer.

    \param p_storage A pointer to a storage.
    \param key       A key.
    \param p_value   The storage for a value.

    \retval true  When the value is set to p_value.
    \retval false Otherwise.
*/
bool tetengo_property_storage_getUint32(
    const tetengo_property_storage_t* p_storage,
    const char*                       key,
    uint32_t*                         p_value);

/*!
    \brief Sets a value in an unsigned 32-bit integer.

    \param p_storage A pointer to a storage.
    \param key       A key.
    \param value     A value.
*/
void tetengo_property_storage_setUint32(tetengo_property_storage_t* p_storage, const char* key, uint32_t value);

/*!
    \brief Returns the value in a string.

    \param p_storage      A pointer to a storage.
    \param key            A key.
    \param p_value        The storage for a value. Can be NULL.
    \param value_capacity A value capacity.

    \return The value length. Or (size_t)-1 when no value for key is found.
*/
size_t tetengo_property_storage_getString(
    const tetengo_property_storage_t* p_storage,
    const char*                       key,
    char*                             p_value,
    size_t                            value_capacity);

/*!
    \brief Sets a value in a string.

    \param p_storage A pointer to a storage.
    \param key       A key.
    \param value     A value.
*/
void tetengo_property_storage_setString(tetengo_property_storage_t* p_storage, const char* key, const char* value);

/*!
    \brief Saves the values.

    \param p_storage A pointer to a storage.
*/
void tetengo_property_storage_save(const tetengo_property_storage_t* p_storage);

/*!
    \brief Creates a memory storage loader.

    \return A pointer to a storage loader.
*/
tetengo_property_storageLoader_t* tetengo_property_storageLoader_createMemoryStorageLoader();

/*!
    \brief Creates a file storage loader.

    \return A pointer to a storage loader.
*/
tetengo_property_storageLoader_t* tetengo_property_storageLoader_createFileStorageLoader();

/*!
    \brief Creates a Windows registry storage loader.

    \return A pointer to a storage loader.
*/
tetengo_property_storageLoader_t* tetengo_property_storageLoader_createWindowsRegistoryStorageLoader();

/*!
    \brief Creates a storage loader proxy.

    The ownership of the real storage loader pointed by p_real_storage_loader is transferred into the storage loader
    proxy.
    There is no need to destroy the real storage loader after calling this function.

    \param p_real_storage_loader A pointer to a real storage loader.

    \return A pointer to a storage loader.
*/
tetengo_property_storageLoader_t*
tetengo_property_storageLoader_createStorageLoaderProxy(tetengo_property_storageLoader_t* p_real_storage_loader);

/*!
    \brief Destroys a storage loader.

    \param p_storage_loader A pointer to a storage loader.
*/
void tetengo_property_storageLoader_destroy(const tetengo_property_storageLoader_t* p_storage_loader);

/*!
    \brief Loads a storage.

    \param p_storage_loader A pointer to a storage loader.
    \param path             A path.

    \return A pointer to a storage. Or NULL on error.
*/
tetengo_property_storage_t*
tetengo_property_storageLoader_load(const tetengo_property_storageLoader_t* p_storage_loader, const char* path);


#if defined(__cplusplus)
}
#endif


#endif
