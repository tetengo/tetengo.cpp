/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_STORAGE_H)
#define TETENGO_PROPERTY_STORAGE_H


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
#if !defined(TYPEDEF_TETENGO_PROPERTY_STORAGE_T)
#define TYPEDEF_TETENGO_PROPERTY_STORAGE_T
typedef struct tetengo_property_storage_tag tetengo_property_storage_t;
#endif
#if !defined(TYPEDEF_TETENGO_PROPERTY_STORAGELOADER_T)
#define TYPEDEF_TETENGO_PROPERTY_STORAGELOADER_T
typedef struct tetengo_property_storageLoader_tag tetengo_property_storageLoader_t;
#endif
#endif

/*!
    \brief Destroys a storage.

    \param p_storage A pointer to a storage.
*/
void tetengo_property_storage_destroy(const tetengo_property_storage_t* p_storage);

/*!
    \brief Creates a memory storage loader.

    \return A pointer to a storage loader.
*/
tetengo_property_storageLoader_t* tetengo_property_storageLoader_createMemoryStorageLoader();

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
