/*! \file
    \brief The usage snippets.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include "usage_tetengo.json.snippets_c.h"

/* [jsonParser] */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <tetengo/json/element.h>
#include <tetengo/json/jsonParser.h>
#include <tetengo/json/reader.h>


int         make_json_file(const char* text, const char* path);
const char* to_string(const tetengo_json_element_t* p_element);

void usage_tetengo_json_jsonParser()
{
    /* clang-format off */
    static const char* const json_text =
            "{\n"
            "  \"hoge\": 42,\n"
            "  \"fuga\": [ \"foo\", \"bar\" ]\n"
            "}\n";
    /* clang-format on */
    static const char* const json_file_path = "jsonParser_sample.json";
    if (!make_json_file(json_text, json_file_path))
    {
        return;
    }
    {
        /* Creates a reader from a file path. */
        tetengo_json_reader_t* const p_reader = tetengo_json_reader_createStreamReader(
            json_file_path, tetengo_json_reader_streamReaderDefaultBufferCapacity());

        /* Creates a JSON parser */
        tetengo_json_jsonParser_t* const p_parser =
            tetengo_json_jsonParser_create(p_reader, tetengo_json_jsonParser_defaultBufferCapacity());

        /* Iteration. */
        char element_list_string[384] = { 0 };
        while (tetengo_json_jsonParser_hasNext(p_parser))
        {
            /* Obtains the current element. */
            const tetengo_json_element_t* const p_element = tetengo_json_jsonParser_peek(p_parser);

            strcat(element_list_string, to_string(p_element));

            /* Moves to the next element. */
            tetengo_json_jsonParser_next(p_parser);
        }

        {
            /* clang-format off */
            static const char* const expected =
                "object:open:\n"
                "member:open:name=hoge:\n"
                "number:42\n"
                "member:close:\n"
                "member:open:name=fuga:\n"
                "array:open:\n"
                "string:foo\n"
                "string:bar\n"
                "array:close:\n"
                "member:close:\n"
                "object:close:\n";
            /* clang-format on */
            assert(strcmp(element_list_string, expected) == 0);
        }

        /* Destroys the JSON parser. The reader inside is also destroyed. */
        tetengo_json_jsonParser_destroy(p_parser);
    }
    remove(json_file_path);
}

int make_json_file(const char* const text, const char* const path)
{
    FILE* const stream = fopen(path, "w");
    if (!stream)
    {
        fprintf(stderr, "Can't create a file: %s", path);
        return 0;
    }
    fputs(text, stream);
    fclose(stream);
    return 1;
}

const char* to_string(const tetengo_json_element_t* const p_element)
{
    static char result[32] = { 0 };
    result[0] = '\0';
    {
        const tetengo_json_element_type_t* const p_type = tetengo_json_element_type(p_element);
        if (p_type->name == tetengo_json_element_typeName_string())
        {
            strcat(result, "string:");
        }
        else if (p_type->name == tetengo_json_element_typeName_number())
        {
            strcat(result, "number:");
        }
        else if (p_type->name == tetengo_json_element_typeName_boolean())
        {
            strcat(result, "boolean:");
        }
        else if (p_type->name == tetengo_json_element_typeName_null())
        {
            strcat(result, "null:");
        }
        else if (p_type->name == tetengo_json_element_typeName_object())
        {
            strcat(result, "object:");
        }
        else if (p_type->name == tetengo_json_element_typeName_member())
        {
            strcat(result, "member:");
        }
        else
        {
            assert(p_type->name == tetengo_json_element_typeName_array());
            strcat(result, "array:");
        }
        if (p_type->category == tetengo_json_element_typeCategory_primitive())
        {
        }
        else if (p_type->category == tetengo_json_element_typeCategory_structureOpen())
        {
            strcat(result, "open:");
        }
        else
        {
            assert(p_type->category == tetengo_json_element_typeCategory_structureClose());
            strcat(result, "close:");
        }
        {
            const char*  key = NULL;
            const size_t attribute_count = tetengo_json_element_attributeKeys(p_element, &key);
            if (attribute_count > 0)
            {
                strcat(result, key);
                strcat(result, "=");
                strcat(result, tetengo_json_element_attributeValueOf(p_element, key));
                strcat(result, ":");
            }
        }
        strcat(result, tetengo_json_element_value(p_element));
    }
    strcat(result, "\n");
    return result;
}
/* [jsonParser] */
