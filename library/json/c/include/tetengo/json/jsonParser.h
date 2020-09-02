/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_JSON_JSONPARSER_H)
#define TETENGO_JSON_JSONPARSER_H


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_json_element_tag    tetengo_json_element_t;
typedef struct tetengo_json_reader_tag     tetengo_json_reader_t;
typedef struct tetengo_json_jsonParser_tag tetengo_json_jsonParser_t;
#endif


/*!
    \brief Creates a JSON parser.

    There is no need to destroy the reader after calling this function.

    \param p_reader A pointer to a reader.

    \return A pointer to a JSON parser. Or NULL when p_reader is NULL.
*/
tetengo_json_jsonParser_t* tetengo_json_jsonParser_create(tetengo_json_reader_t* p_reader);

/*!
    \brief Destroys a JSON parser.

    \param p_parser A pointer to a perser.
*/
void tetengo_json_jsonParser_destroy(const tetengo_json_jsonParser_t* p_parser);


#if defined(__cplusplus)
}
#endif


#endif
