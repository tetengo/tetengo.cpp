/*! \file
    \brief A JSON parser.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_JSONPARSER_H)
#define TETENGO_JSON_JSONPARSER_H

#include <stdbool.h>
#include <stddef.h>

#include <tetengo/json/element.h>
#include <tetengo/json/reader.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_json_jsonParser_tag tetengo_json_jsonParser_t;
#endif


/*!
    \brief Returns the default buffer capacity.

    \return The default buffer capacity.
*/
size_t tetengo_json_jsonParser_defaultBufferCapacity(void);

/*!
    \brief Creates a JSON parser.

    There is no need to destroy the reader after calling this function.

    Based on <a href="https://tools.ietf.org/html/rfc8259">RFC 8259</a>.

    \param p_reader        A pointer to a reader.
    \param buffer_capacity A buffer capacity.

    \return A pointer to a JSON parser. Or NULL when p_reader is NULL.
*/
tetengo_json_jsonParser_t* tetengo_json_jsonParser_create(tetengo_json_reader_t* p_reader, size_t buffer_capacity);

/*!
    \brief Destroys a JSON parser.

    \param p_parser A pointer to a perser.
*/
void tetengo_json_jsonParser_destroy(const tetengo_json_jsonParser_t* p_parser);

/*!
    \brief Returns true when the next element exists.

    \param p_parser A pointer to a perser.

    \retval true  When the next element exists.
    \retval false Otherwise.
*/
bool tetengo_json_jsonParser_hasNext(const tetengo_json_jsonParser_t* p_parser);

/*!
    \brief Returns the current element.

    \param p_parser A pointer to a perser.

    \return The pointer to the current element. Or NULL when p_parser is NULL and/or the current position is beyond the
            termination point.
*/
const tetengo_json_element_t* tetengo_json_jsonParser_peek(const tetengo_json_jsonParser_t* p_parser);

/*!
    \brief Moves to the next element.

    \param p_parser A pointer to a perser.
*/
void tetengo_json_jsonParser_next(tetengo_json_jsonParser_t* p_parser);


#if defined(__cplusplus)
}
#endif


#endif
