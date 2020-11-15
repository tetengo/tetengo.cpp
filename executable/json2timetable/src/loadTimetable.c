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


static const tetengo_json_element_t* get_element(tetengo_json_jsonParser_t* const p_parser)
{
    if (!tetengo_json_jsonParser_hasNext(p_parser))
    {
        return NULL;
    }

    return tetengo_json_jsonParser_peek(p_parser);
}

static size_t min_size(const size_t one, const size_t another)
{
    return one < another ? one : another;
}

static void copy_string(const char* const from, char* const to, const size_t to_capacity)
{
    const size_t length = min_size(strlen(from), to_capacity - 1);
    memcpy(to, from, length);
    to[length] = '\0';
}

static char* duplicate_string(const char* const from)
{
    const size_t length = strlen(from);
    char* const  p_duplication = malloc((length + 1) * sizeof(char));
    copy_string(from, p_duplication, length + 1);
    return p_duplication;
}

static void destroy_string(const void* const string)
{
    free((void*)string);
}

static int* duplicate_int(const char* const from)
{
    int* const p_duplication = malloc(sizeof(int));
    if (!p_duplication)
    {
        return NULL;
    }
    *p_duplication = atoi(from);
    return p_duplication;
}

static void destroy_int(const void* const integer)
{
    free((void*)integer);
}

static const int* create_int_array(const arrayList_t* const p_array_list)
{
    int* const p_array = malloc(sizeof(int) * arrayList_size(p_array_list));
    if (!p_array)
    {
        return NULL;
    }
    {
        size_t i = 0;
        for (i = 0; i < arrayList_size(p_array_list); ++i)
        {
            p_array[i] = *((const int*)arrayList_at(p_array_list, i));
        }
    }
    return p_array;
}

static int element_is(
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

static int element_is_member_begin(
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

static const char* element_is_primitive(
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

static int load_header(
    tetengo_json_jsonParser_t* const   p_parser,
    char* const                        title,
    const size_t                       title_capacity,
    tetengo_json_fileLocation_t* const p_last_file_location)
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
            if (strcmp(key, "title") == 0)
            {
                const char* const value =
                    element_is_primitive(p_parser, tetengo_json_element_typeName_string(), p_last_file_location);
                if (!value)
                {
                    return 0;
                }
                copy_string(value, title, title_capacity);
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
    return 1;
}

static const arrayList_t*
load_stations(tetengo_json_jsonParser_t* const p_parser, tetengo_json_fileLocation_t* const p_last_file_location)
{
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
        return p_stations;
    }
}

typedef struct train_tag
{
    const char*  number;
    arrayList_t* p_stops;
} train_t;

static void destroy_train(const void* const p_train_as_void)
{
    const train_t* const p_train = (const train_t*)p_train_as_void;
    free((void*)p_train->number);
    arrayList_destroy(p_train->p_stops);
    free((void*)p_train);
}

static train_t* load_train(
    tetengo_json_jsonParser_t* const   p_parser,
    const size_t                       station_count,
    tetengo_json_fileLocation_t* const p_last_file_location)
{
    if (!element_is(
            p_parser,
            tetengo_json_element_typeName_object(),
            tetengo_json_element_typeCategory_structureOpen(),
            p_last_file_location))
    {
        return NULL;
    }
    tetengo_json_jsonParser_next(p_parser);
    {
        train_t* const p_train = malloc(sizeof(train_t));
        if (!p_train)
        {
            return NULL;
        }
        p_train->number = NULL;
        p_train->p_stops = arrayList_create(destroy_int);
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
                    destroy_train(p_train);
                    return NULL;
                }
                tetengo_json_jsonParser_next(p_parser);
                if (strcmp(key, "number") == 0)
                {
                    const char* const value =
                        element_is_primitive(p_parser, tetengo_json_element_typeName_string(), p_last_file_location);
                    if (!value)
                    {
                        destroy_train(p_train);
                        return NULL;
                    }
                    p_train->number = duplicate_string(value);
                    tetengo_json_jsonParser_next(p_parser);
                }
                else if (strcmp(key, "stops") == 0)
                {
                    if (!element_is(
                            p_parser,
                            tetengo_json_element_typeName_array(),
                            tetengo_json_element_typeCategory_structureOpen(),
                            p_last_file_location))
                    {
                        destroy_train(p_train);
                        return NULL;
                    }
                    tetengo_json_jsonParser_next(p_parser);
                    {
                        size_t i = 0;
                        for (i = 0; i < station_count; ++i)
                        {
                            const char* const value = element_is_primitive(
                                p_parser, tetengo_json_element_typeName_number(), p_last_file_location);
                            if (!value)
                            {
                                destroy_train(p_train);
                                return NULL;
                            }
                            arrayList_add(p_train->p_stops, duplicate_int(value));
                            tetengo_json_jsonParser_next(p_parser);
                        }
                    }
                    if (!element_is(
                            p_parser,
                            tetengo_json_element_typeName_array(),
                            tetengo_json_element_typeCategory_structureClose(),
                            p_last_file_location))
                    {
                        destroy_train(p_train);
                        return NULL;
                    }
                    tetengo_json_jsonParser_next(p_parser);
                }
                if (!element_is(
                        p_parser,
                        tetengo_json_element_typeName_member(),
                        tetengo_json_element_typeCategory_structureClose(),
                        p_last_file_location))
                {
                    destroy_train(p_train);
                    return NULL;
                }
                tetengo_json_jsonParser_next(p_parser);
            }
        }
        return p_train;
    }
}

static const arrayList_t* load_trains(
    tetengo_json_jsonParser_t* const   p_parser,
    const size_t                       station_count,
    tetengo_json_fileLocation_t* const p_last_file_location)
{
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
        arrayList_t* const p_trains = arrayList_create(destroy_train);
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
                train_t* const p_train = load_train(p_parser, station_count, p_last_file_location);
                if (!p_train)
                {
                    arrayList_destroy(p_trains);
                    return NULL;
                }
                arrayList_add(p_trains, p_train);
            }
        }
        return p_trains;
    }
}

timetable_t* load_timetable(const char* const timetable_file_path)
{
    {
        tetengo_json_reader_t* const p_reader = tetengo_json_reader_createStreamReader(
            timetable_file_path, tetengo_json_reader_streamReaderDefaultBufferCapacity());
        if (!p_reader)
        {
            fprintf(stderr, "Can't open: %s\n", timetable_file_path);
            return NULL;
        }

        {
            tetengo_json_jsonParser_t* const p_parser =
                tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());
            tetengo_json_fileLocation_t last_file_location = { "", 0, 0 };

            if (!element_is(
                    p_parser,
                    tetengo_json_element_typeName_array(),
                    tetengo_json_element_typeCategory_structureOpen(),
                    &last_file_location))
            {
                fprintf(stderr, "JSON syntax error around line %u.\n", (int)last_file_location.line_index);
                tetengo_json_jsonParser_destroy(p_parser);
                return NULL;
            }
            tetengo_json_jsonParser_next(p_parser);
            {
                char title[256] = { 0 };
                if (!load_header(p_parser, title, sizeof(title) / sizeof(char), &last_file_location))
                {
                    fprintf(stderr, "JSON syntax error around line %u.\n", (int)last_file_location.line_index);
                    tetengo_json_jsonParser_destroy(p_parser);
                    return NULL;
                }
                {
                    const arrayList_t* const p_stations = load_stations(p_parser, &last_file_location);
                    if (!p_stations)
                    {
                        fprintf(stderr, "JSON syntax error around line %u.\n", (int)last_file_location.line_index);
                        tetengo_json_jsonParser_destroy(p_parser);
                        return NULL;
                    }
                    {
                        const arrayList_t* const p_trains =
                            load_trains(p_parser, arrayList_size(p_stations), &last_file_location);
                        if (!p_trains)
                        {
                            fprintf(stderr, "JSON syntax error around line %u.\n", (int)last_file_location.line_index);
                            arrayList_destroy(p_stations);
                            tetengo_json_jsonParser_destroy(p_parser);
                            return NULL;
                        }
                        {
                            timetable_t* p_timetable = timetable_create(
                                title, (const char* const*)arrayList_data(p_stations), arrayList_size(p_stations));
                            {
                                size_t i = 0;
                                for (i = 0; i < arrayList_size(p_trains); ++i)
                                {
                                    const train_t* const p_train = arrayList_at(p_trains, i);
                                    const int* const     p_stops = create_int_array(p_train->p_stops);
                                    timetable_addTrain(p_timetable, p_train->number, p_stops);
                                    free((void*)p_stops);
                                }
                            }
                            arrayList_destroy(p_trains);
                            arrayList_destroy(p_stations);

                            if (!element_is(
                                    p_parser,
                                    tetengo_json_element_typeName_array(),
                                    tetengo_json_element_typeCategory_structureClose(),
                                    &last_file_location))
                            {
                                fprintf(
                                    stderr, "JSON syntax error around line %u.\n", (int)last_file_location.line_index);
                                tetengo_json_jsonParser_destroy(p_parser);
                                return NULL;
                            }
                            tetengo_json_jsonParser_next(p_parser);

                            tetengo_json_jsonParser_destroy(p_parser);
                            return p_timetable;
                        }
                    }
                }
            }
        }
    }
}
