/*! \file
    \brief A dictionary search tool.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <assert.h>
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tetengo/text/encoder.h>
#include <tetengo/trie/storage.h>
#include <tetengo/trie/trie.h>


static size_t value_capacity = 4;

static const char* load_lex_csv(const char* const lex_csv_path)
{
    char* lex_csv = NULL;

    FILE* p_stream = fopen(lex_csv_path, "rb");
    if (!p_stream)
    {
        return NULL;
    }

    size_t              lex_csv_size = 0;
    static const size_t buffer_capacity = 1048576;
    char* const         p_buffer = (char*)malloc(buffer_capacity);
    if (!p_buffer)
    {
        return NULL;
    }
    for (;;)
    {
        const size_t read_size = fread(p_buffer, 1, buffer_capacity, p_stream);

        char* const reallocated_lex_csv = (char*)realloc(lex_csv, lex_csv_size + read_size);
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

static const tetengo_trie_trie_t* load_trie(const char* const trie_path)
{
    path_character_type native_path[256] = { 0 };
    to_native_path(trie_path, native_path, sizeof(native_path) / sizeof(path_character_type));
    tetengo_trie_storage_t* const p_storage = tetengo_trie_storage_createMmapStorage(native_path, 0);
    if (!p_storage)
    {
        return NULL;
    }

    const tetengo_trie_trie_t* const p_trie = tetengo_trie_trie_createWithStorage(p_storage);
    return p_trie;
}

void chop_line_feed(char* const line)
{
    const size_t length = strlen(line);
    if (line == 0)
    {
        return;
    }
    if (line[length - 1] != '\n')
    {
        return;
    }

    line[length - 1] = '\0';
}

typedef struct
{
    size_t offset;
    size_t length;
} lex_span_t;

static size_t to_size_t(const char* const p_bytes, size_t* const p_byte_offset)
{
    if (!p_bytes)
    {
        assert(0);
        return 0;
    }
    if (!p_byte_offset)
    {
        assert(0);
        return 0;
    }

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

static size_t lex_span_count(const char* const p_bytes, size_t* const p_byte_offset)
{
    assert(p_bytes);
    assert(p_byte_offset);

    return to_size_t(p_bytes, p_byte_offset);
}

static void to_array_of_lex_span(
    const char* const p_bytes,
    size_t* const     p_byte_offset,
    lex_span_t* const p_lex_spans,
    const size_t      lex_span_count)
{
    assert(p_bytes);
    assert(p_byte_offset);
    if (!p_lex_spans)
    {
        assert(0);
        return;
    }

    for (size_t i = 0; i < lex_span_count; ++i)
    {
        if (i < value_capacity)
        {
            to_lex_span(p_bytes, p_byte_offset, &p_lex_spans[i]);
        }
        else
        {
            p_lex_spans[i].offset = 0;
            p_lex_spans[i].length = 0;
        }
    }
}

static tetengo_text_encoder_encoding_t select_encoding()
{
    const char* const locale = setlocale(LC_CTYPE, NULL);
    if (locale && strcmp(locale, "Japanese_Japan.932") == 0)
    {
        return tetengo_text_encoder_encoding_cp932;
    }
    else
    {
        return tetengo_text_encoder_encoding_utf8;
    }
}

static const char* create_decoded_from_input(const char* const encoded)
{
    const tetengo_text_encoder_t* const p_encoder = tetengo_text_encoder_instance(select_encoding());

    const size_t length = tetengo_text_encoder_decode(p_encoder, encoded, NULL, 0);
    char* const  decoded = malloc((length + 1) * sizeof(char));
    if (!decoded)
    {
        assert(0);
        return NULL;
    }
    tetengo_text_encoder_decode(p_encoder, encoded, decoded, length + 1);
    decoded[length] = '\0';

    return decoded;
}

static const char* create_encoded_for_print(const char* const string)
{
    const tetengo_text_encoder_t* const p_encoder = tetengo_text_encoder_instance(select_encoding());

    const size_t length = tetengo_text_encoder_encode(p_encoder, string, NULL, 0);
    char* const  encoded = malloc((length + 1) * sizeof(char));
    if (!encoded)
    {
        assert(0);
        return NULL;
    }
    tetengo_text_encoder_encode(p_encoder, string, encoded, length + 1);
    encoded[length] = '\0';

    return encoded;
}

int main(const int argc, char** const argv)
{
    setlocale(LC_ALL, "");

    if (argc <= 2)
    {
        fprintf(stderr, "Usage: search_dict_c UniDic_lex.csv trie.bin\n");
        return 0;
    }

    const char* const lex_csv = load_lex_csv(argv[1]);
    if (!lex_csv)
    {
        fprintf(stderr, "Error: Can't open the lex.csv file.\n");
        return 1;
    }

    {
        const tetengo_trie_trie_t* const p_trie = load_trie(argv[2]);
        if (!p_trie)
        {
            fprintf(stderr, "Error: Can't open the trie.bin file.\n");
            free((void*)lex_csv);
            return 1;
        }

        while (!feof(stdin))
        {
            char key[1024] = { 0 };
            printf(">> ");
            if (!fgets(key, 1024, stdin))
            {
                continue;
            }
            chop_line_feed(key);
            if (strlen(key) == 0)
            {
                continue;
            }

            const char* const decoded = create_decoded_from_input(key);
            const void* const p_found = tetengo_trie_trie_find(p_trie, decoded);
            free((void*)decoded);
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

            lex_span_t* const p_lex_spans = (lex_span_t*)malloc(sizeof(lex_span_t) * lex_span_count_);
            if (!p_lex_spans)
            {
                continue;
            }
            to_array_of_lex_span((const char*)p_found, &byte_offset, p_lex_spans, lex_span_count_);
            for (size_t i = 0; i < lex_span_count_; ++i)
            {
                if (p_lex_spans[i].offset == 0 && p_lex_spans[i].length == 0)
                {
                    printf("(truncated)\n");
                }
                else
                {
                    char* const lex = malloc((p_lex_spans[i].length + 1) * sizeof(char));
                    if (lex)
                    {
                        strncpy(lex, &lex_csv[p_lex_spans[i].offset], p_lex_spans[i].length);
                        lex[p_lex_spans[i].length] = '\0';
                        const char* const encoded = create_encoded_for_print(lex);
                        printf("%s", encoded);
                        free((void*)encoded);
                        free(lex);
                    }
                }
            }
            free(p_lex_spans);
        }

        free((void*)p_trie);
        free((void*)lex_csv);

        return 0;
    }
}
