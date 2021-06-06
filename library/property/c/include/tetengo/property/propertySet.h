/*! \file
    \brief A property set.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_PROPERTYSET_H)
#define TETENGO_PROPERTY_PROPERTYSET_H


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
#if !defined(TYPEDEF_TETENGO_PROPERTY_PROPERTYSET_T)
#define TYPEDEF_TETENGO_PROPERTY_PROPERTYSET_T
typedef struct tetengo_property_propertySet_tag tetengo_property_propertySet_t;
#endif
#endif


/*!
    \brief Destroys a property set.

    \param p_property_set A pointer to a property set.
*/
void tetengo_property_propertySet_destroy(const tetengo_property_propertySet_t* p_property_set);


#if defined(__cplusplus)
}
#endif


#endif
