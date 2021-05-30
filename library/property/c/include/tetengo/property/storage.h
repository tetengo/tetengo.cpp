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
typedef struct tetengo_property_storageLoader_tag tetengo_property_storageLoader_t;
#endif
#endif

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


#if defined(__cplusplus)
}
#endif


#endif
