/*! \file
    \brief A timetable printing.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include "printTimetable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tetengo/text/graphemeSplitter.h>


typedef struct grapheme_tag
{
    size_t                         count;
    const tetengo_text_grapheme_t* p_graphemes;
} grapheme_t;

static size_t max_station_name_width(const grapheme_t* const p_station_name_graphemes, const size_t station_count)
{
    size_t max_width = 0;
    size_t i = 0;
    for (i = 0; i < station_count; ++i)
    {
        size_t width = 0;
        size_t j = 0;
        for (j = 0; j < p_station_name_graphemes[i].count; ++j)
        {
            width += p_station_name_graphemes[i].p_graphemes[j].width;
        }
        if (width > max_width)
        {
            max_width = width;
        }
    }
    return max_width;
}

static const char*
create_equal_spaced(const char* const string, const grapheme_t* const p_graphemes, const size_t max_width)
{
    size_t width = 0;
    {
        size_t i = 0;
        for (i = 0; i < p_graphemes->count; ++i)
        {
            width += p_graphemes->p_graphemes[i].width;
        }
    }
    {
        char* const p_equal_escaped = malloc((strlen(string) + max_width - width + 1) * sizeof(char));
        if (!p_equal_escaped)
        {
            return NULL;
        }
        if (p_graphemes->count == 1)
        {
            size_t equal_spaced_offset = 0;
            {
                const size_t space_count = (max_width - width) / 2;
                size_t       i = 0;
                for (i = 0; i < space_count; ++i)
                {
                    p_equal_escaped[equal_spaced_offset + i] = ' ';
                }
                equal_spaced_offset += space_count;
            }
            {
                const size_t character_length = strlen(string);
                memcpy(p_equal_escaped + equal_spaced_offset, string, character_length);
                equal_spaced_offset += character_length;
            }
            {
                const size_t space_count = max_width - width - (max_width - width) / 2;
                size_t       i = 0;
                for (i = 0; i < space_count; ++i)
                {
                    p_equal_escaped[equal_spaced_offset + i] = ' ';
                }
                equal_spaced_offset += space_count;
            }
            p_equal_escaped[equal_spaced_offset] = '\0';
        }
        else
        {
            const size_t min_space_count = (max_width - width) / (p_graphemes->count - 1);
            const size_t space_remainder = (max_width - width) % (p_graphemes->count - 1);
            size_t       equal_spaced_offset = 0;
            size_t       i = 0;
            for (i = 0; i < p_graphemes->count - 1; ++i)
            {
                {
                    const size_t character_length =
                        p_graphemes->p_graphemes[i + 1].offset - p_graphemes->p_graphemes[i].offset;
                    memcpy(
                        p_equal_escaped + equal_spaced_offset,
                        string + p_graphemes->p_graphemes[i].offset,
                        character_length);
                    equal_spaced_offset += character_length;
                }
                {
                    size_t j = 0;
                    for (j = 0; j < min_space_count; ++j)
                    {
                        p_equal_escaped[equal_spaced_offset + j] = ' ';
                    }
                    equal_spaced_offset += min_space_count;
                }
                if (p_graphemes->count - 1 - i <= space_remainder)
                {
                    p_equal_escaped[equal_spaced_offset] = ' ';
                    ++equal_spaced_offset;
                }
            }
            {
                const size_t character_length =
                    strlen(string) - p_graphemes->p_graphemes[p_graphemes->count - 1].offset;
                memcpy(
                    p_equal_escaped + equal_spaced_offset,
                    string + p_graphemes->p_graphemes[p_graphemes->count - 1].offset,
                    character_length);
                equal_spaced_offset += character_length;
            }
            p_equal_escaped[equal_spaced_offset] = '\0';
        }
        return p_equal_escaped;
    }
}

static const char* const* create_station_display_names(const timetable_t* const p_timetable, size_t* const p_max_width)
{
    const size_t      station_count = timetable_stationCount(p_timetable);
    grapheme_t* const p_station_name_graphemes = malloc(sizeof(grapheme_t) * station_count);
    if (!p_station_name_graphemes)
    {
        return NULL;
    }
    {
        const tetengo_text_graphemeSplitter_t* const p_grapheme_splitter = tetengo_text_graphemeSplitter_create();
        size_t                                       i = 0;
        for (i = 0; i < station_count; ++i)
        {
            const char* const name = timetable_stationAt(p_timetable, i);
            const size_t      grapheme_count = tetengo_text_graphemeSplitter_split(p_grapheme_splitter, name, NULL);
            tetengo_text_grapheme_t* const p_graphemes = malloc(sizeof(tetengo_text_grapheme_t) * grapheme_count);
            tetengo_text_graphemeSplitter_split(p_grapheme_splitter, name, p_graphemes);

            p_station_name_graphemes[i].count = grapheme_count;
            p_station_name_graphemes[i].p_graphemes = p_graphemes;
        }
        tetengo_text_graphemeSplitter_destroy(p_grapheme_splitter);
    }

    {
        const size_t       max_width = max_station_name_width(p_station_name_graphemes, station_count);
        const char** const pp_station_display_names = malloc(sizeof(const char*) * station_count);
        if (pp_station_display_names)
        {
            {
                {
                    size_t i = 0;
                    for (i = 0; i < station_count; ++i)
                    {
                        pp_station_display_names[i] = create_equal_spaced(
                            timetable_stationAt(p_timetable, i), &p_station_name_graphemes[i], max_width);
                    }
                }
            }
        }

        {
            size_t i = 0;
            for (i = 0; i < station_count; ++i)
            {
                free((void*)p_station_name_graphemes[i].p_graphemes);
            }
            free(p_station_name_graphemes);
        }

        if (p_max_width)
        {
            *p_max_width = max_width;
        }
        return pp_station_display_names;
    }
}

static void print_horizontal_line(const timetable_t* const p_timetable, const size_t max_station_name_width)
{
    printf("+");
    {
        size_t i = 0;
        for (i = 0; i < max_station_name_width; ++i)
        {
            printf("-");
        }
    }
    printf("+");
    {
        size_t i = 0;
        for (i = 0; i < timetable_trainCount(p_timetable); ++i)
        {
            printf("----+");
        }
    }
    printf("\n");
}

static const char* create_train_display_name(const char* const name)
{
    const tetengo_text_graphemeSplitter_t* const p_grapheme_splitter = tetengo_text_graphemeSplitter_create();
    const size_t grapheme_count = tetengo_text_graphemeSplitter_split(p_grapheme_splitter, name, NULL);
    tetengo_text_grapheme_t* const p_graphemes = malloc(sizeof(tetengo_text_grapheme_t) * grapheme_count);
    if (!p_graphemes)
    {
        tetengo_text_graphemeSplitter_destroy(p_grapheme_splitter);
        return NULL;
    }
    tetengo_text_graphemeSplitter_split(p_grapheme_splitter, name, p_graphemes);

    {
        size_t width = 0;
        size_t display_name_length = strlen(name);
        size_t i = 0;
        for (i = 0; i < grapheme_count; ++i)
        {
            if (width + p_graphemes[i].width > 4)
            {
                display_name_length = p_graphemes[i].offset;
                break;
            }
            width += p_graphemes[i].width;
        }

        free(p_graphemes);
        tetengo_text_graphemeSplitter_destroy(p_grapheme_splitter);

        {
            char* const display_name = malloc((display_name_length + 4 - width + 1) * sizeof(char));
            {
                size_t j = 0;
                for (j = 0; j < 4 - width; ++j)
                {
                    display_name[j] = ' ';
                }
            }
            strncpy(display_name + (4 - width) * sizeof(char), name, display_name_length);
            display_name[display_name_length + 4 - width] = '\0';

            return display_name;
        }
    }
}

static void print_train_names(const timetable_t* const p_timetable, const size_t max_station_name_width)
{
    printf("|");
    {
        size_t i = 0;
        for (i = 0; i < max_station_name_width; ++i)
        {
            printf(" ");
        }
    }
    printf("|");
    {
        size_t i = 0;
        for (i = 0; i < timetable_trainCount(p_timetable); ++i)
        {
            const char* const display_name = create_train_display_name(timetable_trainNameAt(p_timetable, i));
            printf("%4s|", display_name);
            free((void*)display_name);
        }
    }
    printf("\n");
}

static void print_station_name_and_train_times(
    const timetable_t* const p_timetable,
    const char* const* const p_station_display_names)
{
    const size_t train_count = timetable_trainCount(p_timetable);
    int* const   p_passings = malloc(sizeof(int) * train_count);
    if (!p_passings)
    {
        return;
    }
    {
        size_t i = 0;
        for (i = 0; i < train_count; ++i)
        {
            p_passings[i] = 0;
        }
    }
    {
        const size_t station_count = timetable_stationCount(p_timetable);
        size_t       i = 0;
        for (i = 0; i < station_count; ++i)
        {
            {
                printf("|%s|", p_station_display_names[i]);
            }
            {
                size_t j = 0;
                for (j = 0; j < train_count; ++j)
                {
                    const int time = timetable_trainTimeAt(p_timetable, j, i);
                    if (time >= 0)
                    {
                        printf("%4.3d|", time);
                        p_passings[j] = 1;
                        continue;
                    }

                    if (p_passings[i])
                    {
                        int    after_destination = 1;
                        size_t k = 0;
                        for (k = i + 1; k < station_count; ++k)
                        {
                            if (timetable_trainTimeAt(p_timetable, j, k) >= 0)
                            {
                                after_destination = 0;
                                break;
                            }
                        }
                        if (after_destination)
                        {
                            if (i > 0 && timetable_trainTimeAt(p_timetable, j, i - 1) >= 0)
                            {
                                printf("====|");
                            }
                            else
                            {
                                printf("    |");
                            }
                            continue;
                        }
                        else
                        {
                            printf("  | |");
                        }
                    }
                    else
                    {
                        printf("    |");
                    }
                }
            }
            printf("\n");
        }
    }
    free(p_passings);
}

void print_line_timetable(const timetable_t* const p_timetable)
{
    if (!p_timetable)
    {
        return;
    }

    {
        size_t                   max_width = 0;
        const char* const* const p_station_display_names = create_station_display_names(p_timetable, &max_width);

        print_horizontal_line(p_timetable, max_width);
        print_train_names(p_timetable, max_width);
        print_horizontal_line(p_timetable, max_width);
        print_station_name_and_train_times(p_timetable, p_station_display_names);
        print_horizontal_line(p_timetable, max_width);

        {
            size_t i = 0;
            for (i = 0; i < timetable_stationCount(p_timetable); ++i)
            {
                free((void*)p_station_display_names[i]);
            }
        }
        free((void*)p_station_display_names);
    }
}
