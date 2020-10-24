/*! \file
    \brief A timetable loading.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "loadTimetable.h"

#include <stdio.h>

#include <tetengo/json/element.h>
#include <tetengo/json/jsonParser.h>
#include <tetengo/json/reader.h>

#include "timetable.h"


timetable_t* load_timetable(const char* const timetable_file_path)
{
    {
        tetengo_json_reader_t* const p_reader =
            tetengo_json_reader_createLineCountingReader(tetengo_json_reader_createStreamReader(
                timetable_file_path, tetengo_json_reader_streamReaderDefaultBufferCapacity()));

        tetengo_json_jsonParser_t* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());

        while (tetengo_json_jsonParser_hasNext(p_parser))
        {
            const tetengo_json_element_t* const p_element = tetengo_json_jsonParser_peek(p_parser);
            /* tetengo_json_location_t             location = { 0 }; */
            /* tetengo_json_reader_getLocation(p_reader, &location); */

            const tetengo_json_element_type_t* const p_element_type = tetengo_json_element_type(p_element);
            printf("%d: %s\n", p_element_type->name, tetengo_json_element_value(p_element));
            /* printf("  %.*s (%d)\n", (int)location.line_length, location.line, (int)location.column_index); */

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
