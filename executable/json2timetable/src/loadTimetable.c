/*! \file
    \brief A timetable loading.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "loadTimetable.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tetengo/json/element.h>
#include <tetengo/json/jsonParser.h>
#include <tetengo/json/reader.h>

#include "arrayList.h"
#include "timetable.h"


const tetengo_json_element_t* get_element(tetengo_json_jsonParser_t* const p_parser)
{
    if (!tetengo_json_jsonParser_hasNext(p_parser))
    {
        return NULL;
    }

    return tetengo_json_jsonParser_peek(p_parser);
}

size_t min_size(const size_t one, const size_t another)
{
    return one < another ? one : another;
}

void copy_string(const char* const from, char* const to, size_t to_capacity)
{
    const size_t length = min_size(strlen(from), to_capacity - 1);
    memcpy(to, from, length);
    to[length] = '\0';
}

char* duplicate_string(const char* const from)
{
    const size_t length = strlen(from);
    char* const  p_duplication = malloc((length + 1) * sizeof(char));
    copy_string(from, p_duplication, length + 1);
    return p_duplication;
}

void destroy_string(const void* string)
{
    free((void*)string);
}

int element_is(
    tetengo_json_jsonParser_t* const   p_parser,
    const int                          type_name,
    const int                          type_category,
    tetengo_json_fileLocation_t* const p_last_file_location)
{
    const tetengo_json_element_t* const p_element = get_element(p_parser);
    if (!p_element)
    {
        return 0;
    }
    {
        const tetengo_json_element_type_t* const p_type = tetengo_json_element_type(p_element);
        if (!p_type)
        {
            return 0;
        }
        if (p_type->name != type_name || p_type->category != type_category)
        {
            return 0;
        }
    }
    assert(p_last_file_location);
    *p_last_file_location = *tetengo_json_element_getFileLocation(p_element);
    return 1;
}

int element_is_member_begin(
    tetengo_json_jsonParser_t* const   p_parser,
    char* const                        key,
    const size_t                       key_capacity,
    tetengo_json_fileLocation_t* const p_last_file_location)
{
    const tetengo_json_element_t* const p_element = get_element(p_parser);
    if (!p_element)
    {
        return 0;
    }
    {
        const tetengo_json_element_type_t* const p_type = tetengo_json_element_type(p_element);
        if (!p_type)
        {
            return 0;
        }
        if (p_type->name != tetengo_json_element_typeName_member() ||
            p_type->category != tetengo_json_element_typeCategory_structureOpen())
        {
            return 0;
        }
    }
    {
        const size_t attribute_count = tetengo_json_element_attributeKeys(p_element, NULL);
        if (attribute_count != 1)
        {
            return 0;
        }
        {
            const char* attribute_key = NULL;
            tetengo_json_element_attributeKeys(p_element, &attribute_key);
            {
                const char* const attribute_value = tetengo_json_element_attributeValueOf(p_element, attribute_key);
                copy_string(attribute_value, key, key_capacity);
            }
        }
    }
    assert(p_last_file_location);
    *p_last_file_location = *tetengo_json_element_getFileLocation(p_element);
    return 1;
}

const char* element_is_primitive(
    tetengo_json_jsonParser_t* const   p_parser,
    const int                          type_name,
    tetengo_json_fileLocation_t* const p_last_file_location)
{
    const tetengo_json_element_t* const p_element = get_element(p_parser);
    if (!p_element)
    {
        return 0;
    }
    {
        const tetengo_json_element_type_t* const p_type = tetengo_json_element_type(p_element);
        if (!p_type)
        {
            return 0;
        }
        if (p_type->name != type_name || p_type->category != tetengo_json_element_typeCategory_primitive())
        {
            return 0;
        }
    }
    assert(p_last_file_location);
    *p_last_file_location = *tetengo_json_element_getFileLocation(p_element);
    return tetengo_json_element_value(p_element);
}

int load_preamble(tetengo_json_jsonParser_t* const p_parser, tetengo_json_fileLocation_t* const p_last_file_location)
{
    if (!element_is(
            p_parser,
            tetengo_json_element_typeName_object(),
            tetengo_json_element_typeCategory_structureOpen(),
            p_last_file_location))
    {
        return 0;
    }
    tetengo_json_jsonParser_next(p_parser);
    return 1;
}

int load_header(
    tetengo_json_jsonParser_t* const   p_parser,
    char* const                        title,
    const size_t                       title_capacity,
    tetengo_json_fileLocation_t* const p_last_file_location)
{
    {
        char key[256] = { 0 };
        if (!element_is_member_begin(p_parser, key, sizeof(key) / sizeof(char), p_last_file_location))
        {
            return 0;
        }
        if (strcmp(key, "header") != 0)
        {
            return 0;
        }
        tetengo_json_jsonParser_next(p_parser);
    }
    if (!element_is(
            p_parser,
            tetengo_json_element_typeName_object(),
            tetengo_json_element_typeCategory_structureOpen(),
            p_last_file_location))
    {
        return 0;
    }
    tetengo_json_jsonParser_next(p_parser);

    for (;;)
    {
        if (element_is(
                p_parser,
                tetengo_json_element_typeName_object(),
                tetengo_json_element_typeCategory_structureClose(),
                p_last_file_location))
        {
            tetengo_json_jsonParser_next(p_parser);
            break;
        }
        {
            char key[256] = { 0 };
            if (!element_is_member_begin(p_parser, key, sizeof(key) / sizeof(char), p_last_file_location))
            {
                return 0;
            }
            tetengo_json_jsonParser_next(p_parser);
            {
                const char* const value =
                    element_is_primitive(p_parser, tetengo_json_element_typeName_string(), p_last_file_location);
                if (!value)
                {
                    return 0;
                }
                if (strcmp(key, "title") == 0)
                {
                    copy_string(value, title, title_capacity);
                }
                tetengo_json_jsonParser_next(p_parser);
            }
            if (!element_is(
                    p_parser,
                    tetengo_json_element_typeName_member(),
                    tetengo_json_element_typeCategory_structureClose(),
                    p_last_file_location))
            {
                return 0;
            }
            tetengo_json_jsonParser_next(p_parser);
        }
    }

    if (!element_is(
            p_parser,
            tetengo_json_element_typeName_member(),
            tetengo_json_element_typeCategory_structureClose(),
            p_last_file_location))
    {
        return 0;
    }
    tetengo_json_jsonParser_next(p_parser);

    return 1;
}

const arrayList_t*
load_stations(tetengo_json_jsonParser_t* const p_parser, tetengo_json_fileLocation_t* const p_last_file_location)
{
    {
        char key[256] = { 0 };
        if (!element_is_member_begin(p_parser, key, sizeof(key) / sizeof(char), p_last_file_location))
        {
            return NULL;
        }
        if (strcmp(key, "stations") != 0)
        {
            return NULL;
        }
        tetengo_json_jsonParser_next(p_parser);
    }
    if (!element_is(
            p_parser,
            tetengo_json_element_typeName_array(),
            tetengo_json_element_typeCategory_structureOpen(),
            p_last_file_location))
    {
        return NULL;
    }
    tetengo_json_jsonParser_next(p_parser);

    {
        arrayList_t* const p_stations = arrayList_create(destroy_string);

        for (;;)
        {
            if (element_is(
                    p_parser,
                    tetengo_json_element_typeName_array(),
                    tetengo_json_element_typeCategory_structureClose(),
                    p_last_file_location))
            {
                tetengo_json_jsonParser_next(p_parser);
                break;
            }
            {
                const char* const value =
                    element_is_primitive(p_parser, tetengo_json_element_typeName_string(), p_last_file_location);
                if (!value)
                {
                    arrayList_destroy(p_stations);
                    return NULL;
                }
                arrayList_add(p_stations, duplicate_string(value));
                tetengo_json_jsonParser_next(p_parser);
            }
        }

        if (!element_is(
                p_parser,
                tetengo_json_element_typeName_member(),
                tetengo_json_element_typeCategory_structureClose(),
                p_last_file_location))
        {
            arrayList_destroy(p_stations);
            return NULL;
        }
        tetengo_json_jsonParser_next(p_parser);

        return p_stations;
    }
}


timetable_t* load_timetable(const char* const timetable_file_path)
{
    {
        tetengo_json_reader_t* const p_reader = tetengo_json_reader_createStreamReader(
            timetable_file_path, tetengo_json_reader_streamReaderDefaultBufferCapacity());
        tetengo_json_jsonParser_t* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
        tetengo_json_fileLocation_t last_file_location = { "", 0, 0 };

        if (!load_preamble(p_parser, &last_file_location))
        {
            fprintf(stderr, "JSON syntax error around line %u.", (int)last_file_location.line_index + 1);
            tetengo_json_jsonParser_destroy(p_parser);
            return NULL;
        }
        {
            char title[256] = { 0 };
            if (!load_header(p_parser, title, sizeof(title) / sizeof(char), &last_file_location))
            {
                fprintf(stderr, "JSON syntax error around line %u.", (int)last_file_location.line_index + 1);
                tetengo_json_jsonParser_destroy(p_parser);
                return NULL;
            }
            {
                const arrayList_t* const p_stations = load_stations(p_parser, &last_file_location);
                if (!p_stations)
                {
                    fprintf(stderr, "JSON syntax error around line %u.", (int)last_file_location.line_index + 1);
                    tetengo_json_jsonParser_destroy(p_parser);
                    return NULL;
                }
                {
                    timetable_t* p_timetable =
                        timetable_create((const char* const*)arrayList_data(p_stations), arrayList_size(p_stations));

                    arrayList_destroy(p_stations);
                    tetengo_json_jsonParser_destroy(p_parser);
                    return p_timetable;
                }
            }
        }
    }
#if 0
    {
        static const char* p_stations[] = { "HogeHoge", "F", "Piyo" };
        timetable_t*       p_timetable = timetable_create(p_stations, sizeof(p_stations) / sizeof(const char*));

        {
            static const int times[] = { -1, 600, 730 };
            timetable_addTrain(p_timetable, "101M", times);
        }
        {
            static const int times[] = { 1000, 1200, 1330 };
            timetable_addTrain(p_timetable, "103M", times);
        }
        {
            static const int times[] = { 1600, -1, 1900 };
            timetable_addTrain(p_timetable, "5M", times);
        }
        {
            static const int times[] = { 2200, 0, -1 };
            timetable_addTrain(p_timetable, "3107M", times);
        }

        return p_timetable;
    }
#endif
}
