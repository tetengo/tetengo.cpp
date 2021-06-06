/*! \file
    \brief A property set.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_PROPERTYSET_H)
#define TETENGO_PROPERTY_PROPERTYSET_H


#if defined(__cplusplus)
extern "C" {
#endif

#include <tetengo/property/storage.h>

#if !defined(DOCUMENTATION)
#if !defined(TYPEDEF_TETENGO_PROPERTY_PROPERTYSET_T)
#define TYPEDEF_TETENGO_PROPERTY_PROPERTYSET_T
typedef struct tetengo_property_propertySet_tag tetengo_property_propertySet_t;
#endif
#if !defined(TYPEDEF_TETENGO_PROPERTY_STORAGELOADER_T)
#define TYPEDEF_TETENGO_PROPERTY_STORAGELOADER_T
typedef struct tetengo_property_storageLoader_tag tetengo_property_storageLoader_t;
#endif
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


#if defined(__cplusplus)
}
#endif


#endif
