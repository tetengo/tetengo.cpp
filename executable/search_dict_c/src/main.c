/*! \file
    \brief A dictionary search tool.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static const char* load_lex_csv(const char* lex_csv_path)
{
    FILE* p_stream = fopen(lex_csv_path, "rb");
    if (!p_stream)
    {
        return NULL;
    }

    char*               lex_csv = NULL;
    size_t              lex_csv_size = 0;
    static const size_t buffer_capacity = 1048576;
    char* const         p_buffer = malloc(buffer_capacity);
    if (!p_buffer)
    {
        return NULL;
    }
    for (;;)
    {
        const size_t read_size = fread(p_buffer, 1, buffer_capacity, p_stream);

        char* const reallocated_lex_csv = realloc(lex_csv, lex_csv_size + read_size);
        if (!reallocated_lex_csv)
        {
            free(lex_csv);
            return NULL;
        }
        lex_csv = reallocated_lex_csv;
        memcpy(lex_csv + lex_csv_size, p_buffer, read_size);
        lex_csv_size += read_size;

        if (read_size < buffer_capacity)
        {
            break;
        }
    }
    free(p_buffer);

    fclose(p_stream);

    return lex_csv;
}


int main(const int argc, char** const argv)
{
    if (argc <= 2)
    {
        printf("Usage: search_dict_c UniDic_lex.csv trie.bin\n");
        return 0;
    }

    const char* const lex_csv = load_lex_csv(argv[1]);
    if (!lex_csv)
    {
        fprintf(stderr, "Can't open the lex.csv file.\n");
        return 1;
    }

    free((void*)lex_csv);

    return 0;
}
