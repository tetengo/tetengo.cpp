/*! \file
    \brief A timetable loading.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "loadTimetable.h"

#include <assert.h>
#include <stdio.h>

#include <tetengo/json/element.h>
#include <tetengo/json/jsonParser.h>
#include <tetengo/json/reader.h>

#include "timetable.h"


int load_preamble(tetengo_json_jsonParser_t* const p_parser, tetengo_json_fileLocation_t* p_last_file_location)
{
    assert(p_last_file_location);

    if (!tetengo_json_jsonParser_hasNext(p_parser))
    {
        return 0;
    }

    {
        const tetengo_json_element_t* const p_element = tetengo_json_jsonParser_peek(p_parser);
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
            if (p_type->name != tetengo_json_element_typeName_object() ||
                p_type->category != tetengo_json_element_typeCategory_structureOpen())
            {
                return 0;
            }
        }

        *p_last_file_location = *tetengo_json_element_getFileLocation(p_element);
    }

    return 1;
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
            fprintf(stderr, "JSON syntax error around line %u.", (int)last_file_location.line_index);
            return NULL;
        }

        while (tetengo_json_jsonParser_hasNext(p_parser))
        {
            const tetengo_json_element_t* const p_element = tetengo_json_jsonParser_peek(p_parser);
            if (!p_element)
            {
                break;
            }

            tetengo_json_jsonParser_next(p_parser);
        }

        tetengo_json_jsonParser_destroy(p_parser);
    }

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
}
