/*! \file
    \brief A dictionary search tool.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tetengo/trie/storage.h>
#include <tetengo/trie/trie.h>


static const char* load_lex_csv(const char* const lex_csv_path)
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

static void to_native_path(const char* const path, path_character_type* native_path, const size_t native_path_capacity)
{
    const size_t path_length = strlen(path);
    const size_t length = path_length < native_path_capacity - 1 ? path_length : native_path_capacity - 1;
    for (size_t i = 0; i < length; ++i)
    {
        native_path[i] = path[i];
    }
}

static const tetengo_trie_trie* load_trie(const char* const trie_path)
{
    path_character_type native_path[256] = { 0 };
    to_native_path(trie_path, native_path, sizeof(native_path) / sizeof(path_character_type));
    tetengo_trie_storage* const p_storage = tetengo_trie_storage_createMemoryStorage(native_path);
    if (!p_storage)
    {
        return NULL;
    }

    const tetengo_trie_trie* const p_trie = tetengo_trie_trie_createWithStorage(p_storage);
    return p_trie;
}

typedef struct
{
    size_t offset;
    size_t length;
} lex_span_t;

static size_t to_size_t(const char* const p_bytes, size_t* const p_byte_offset)
{
    assert(p_bytes);
    assert(p_byte_offset);

    size_t value = 0;
    for (size_t i = 0; i < 4; ++i)
    {
        value <<= 8;
        value |= (unsigned char)p_bytes[*p_byte_offset + i];
    }
    *p_byte_offset += 4;
    return value;
}

static void to_lex_span(const char* const p_bytes, size_t* const p_byte_offset, lex_span_t* const p_result)
{
    assert(p_bytes);
    assert(p_byte_offset);

    p_result->offset = to_size_t(p_bytes, p_byte_offset);
    p_result->length = to_size_t(p_bytes, p_byte_offset);
}

static size_t lex_span_count(const void* const p_bytes, size_t* const p_byte_offset)
{
    assert(p_bytes);
    assert(p_byte_offset);

    return to_size_t(p_bytes, p_byte_offset);
}

static void to_array_of_lex_span(
    const void* const p_bytes,
    size_t* const     p_byte_offset,
    lex_span_t* const p_lex_spans,
    const size_t      lex_span_count)
{
    assert(p_bytes);
    assert(p_byte_offset);
    assert(p_lex_spans);

    for (size_t i = 0; i < lex_span_count; ++i)
    {
        to_lex_span(p_bytes, p_byte_offset, &p_lex_spans[i]);
    }
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

    const tetengo_trie_trie* const p_trie = load_trie(argv[2]);
    if (!p_trie)
    {
        fprintf(stderr, "Can't open the trie.bin file.\n");
        free((void*)lex_csv);
        return 1;
    }

    while (!feof(stdin))
    {
        printf(">> ");
        char key[1024] = { 0 };
        fgets(key, 1024, stdin);
        size_t key_length = strlen(key);
        if (key_length > 0 && key[key_length - 1] =='\n')
        {
            key[key_length - 1] = '\0';
            --key_length;
        }
        if (key_length == 0)
        {
            continue;
        }

        const void* const p_found = tetengo_trie_trie_find(p_trie, key);
        if (!p_found)
        {
            printf("ERROR: Not found.\n");
            continue;
        }

        size_t       byte_offset = 0;
        const size_t lex_span_count_ = lex_span_count((const char*)p_found, &byte_offset);
        if (lex_span_count_ == 0)
        {
            continue;
        }

        lex_span_t* const p_lex_spans = malloc(sizeof(lex_span_t) * lex_span_count_);
        to_array_of_lex_span((const char*)p_found, &byte_offset, p_lex_spans, lex_span_count_);
        for (size_t i = 0; i < lex_span_count_; ++i)
        {
            printf("(%zd, %zd)\n", p_lex_spans[i].offset, p_lex_spans[i].length);
        }

        free(p_lex_spans);
    }

    free((void*)p_trie);
    free((void*)lex_csv);

    return 0;
}
