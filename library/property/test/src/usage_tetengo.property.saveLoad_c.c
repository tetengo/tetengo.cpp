/*! \file
    \brief The usage of property saving and loading.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include "usage_tetengo.property.saveLoad_c.h"

/* [save_load] */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <tetengo/property/propertySet.h>
#include <tetengo/property/storage.h>


static const char* setting_file_path();

static int file_exists(const char* path);

void usage_tetengo_property_saveLoad()
{
    assert(!file_exists(setting_file_path()));

    {
        /* Creates separate property sets with the same path for illustration purpose. */
        /* Uses the one for setting and the other for getting. */
        tetengo_property_propertySet_t* const p_props_for_setting = tetengo_property_propertySet_create(
            tetengo_property_storageLoader_createFileStorageLoader(), "tetengo_property_sample");
        tetengo_property_propertySet_t* const p_props_for_getting = tetengo_property_propertySet_create(
            tetengo_property_storageLoader_createFileStorageLoader(), "tetengo_property_sample");

        /*
            On Windows, by using tetengo_property_storageLoader_createWindowsRegistoryStorageLoader(), the values will
            be stored into the Windows registry.
        */
        /*
        tetengo_property_propertySet_t* const p_props_for_setting = tetengo_property_propertySet_create(
            tetengo_property_storageLoader_createWindowsRegistoryStorageLoader(), "tetengo_property_sample");
        tetengo_property_propertySet_t* const p_props_for_getting = tetengo_property_propertySet_create(
            tetengo_property_storageLoader_createWindowsRegistoryStorageLoader(), "tetengo_property_sample");
        */

        /* Sets a string value "bar" for the key "foo". */
        tetengo_property_propertySet_setString(p_props_for_setting, "foo", "bar");

        {
            /* The value cannot be yet obtained from the property set for getting. */
            const size_t value_length1 = tetengo_property_propertySet_getString(p_props_for_getting, "foo", NULL, 0);
            (void)value_length1;
            assert(value_length1 == (size_t)-1);
        }

        /* Saves the values of the property set for setting to the file. */
        tetengo_property_propertySet_commit(p_props_for_setting);
        assert(file_exists(setting_file_path()));

        {
            /* The value cannot be yet obtained from the property set for getting. */
            const size_t value_length2 = tetengo_property_propertySet_getString(p_props_for_getting, "foo", NULL, 0);
            (void)value_length2;
            assert(value_length2 == (size_t)-1);
        }

        /* Loads the property set values from the file. */
        tetengo_property_propertySet_update(p_props_for_getting);

        {
            /* The value can now be obtained from the property set for getting. */
            const size_t value_length3 = tetengo_property_propertySet_getString(p_props_for_getting, "foo", NULL, 0);
            assert(value_length3 != (size_t)-1);
            {
                char* const value = (char*)malloc((value_length3 + 1) * sizeof(char));
                if (!value)
                {
                    assert(0);
                    return;
                }
                value[value_length3] = '\0';
                tetengo_property_propertySet_getString(p_props_for_getting, "foo", value, value_length3 + 1);
                assert(strcmp(value, "bar") == 0);
                free((void*)value);
            }
        }

        remove(setting_file_path());

        tetengo_property_propertySet_destroy(p_props_for_getting);
        tetengo_property_propertySet_destroy(p_props_for_setting);
    }
}

static const char* setting_file_path()
{
    static char path[4096] = { '\0' };
    if (path[0] == '\0')
    {
        /*
            When specifying the path "tetengo_property_sample" (with no path delimiter),
        */
#if defined(_WIN32)
        /*
            in the usual case on Windows, the path of the setting file will be
            "C:\Users\USER_NAME\AppData\Roaming\tetengo_property_sample\tetengo_property_sample.json".
        */
        strcat(path, getenv("APPDATA"));
        strcat(path, "\\tetengo_property_sample\\tetengo_property_sample.json");
#else
        /*
            in the usual case on UNIX, the path of the setting file will be
            "/home/USER_NAME/.tetengo_property_sample.json".
        */
        strcat(path, getenv("HOME"));
        strcat(path, "/.tetengo_property_sample.json");
#endif
    }
    return path;
}

static int file_exists(const char* const path)
{
#if defined(_WIN32)
    struct _stat buffer;
    return _stat(path, &buffer) == 0;
#else
    struct stat buffer;
    return stat(path, &buffer) == 0;
#endif
}
/* [save_load] */
