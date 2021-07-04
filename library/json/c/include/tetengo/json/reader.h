/*! \file
    \brief A reader.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_READER_H)
#define TETENGO_JSON_READER_H

#include <stddef.h>


#if defined(__cplusplus)
extern "C" {
#endif

typedef struct tetengo_json_reader_tag tetengo_json_reader_t;


/*!
    \brief A location.
*/
typedef struct tetengo_json_location_tag
{
    /*! A line. Not necessarily NUL-terminated. */
    const char* line;

    /*! A line length. */
    size_t line_length;

    /*! A line index. */
    size_t line_index;

    /*! A column index. */
    size_t column_index;


} tetengo_json_location_t;

/*!
    \brief Returns the default buffer capacity of the stream reader.

    \return The default buffer capacity.
*/
size_t tetengo_json_reader_streamReaderDefaultBufferCapacity();

/*!
    \brief Creates a stream reader.

    \param file_path       A file path.
    \param buffer_capacity A buffer capacity.

    \return A pointer to a reader. Or NULL when the file cannot be opened or buffer_capacity is 0.
*/
tetengo_json_reader_t* tetengo_json_reader_createStreamReader(const char* file_path, size_t buffer_capacity);

/*!
    \brief Creates a comment removing reader.

    There is no need to destroy the base reader after calling this function.

    \param p_base_reader     A pointer to a base reader.
    \param single_line_begin A beginning of a single line comment.

    \return A pointer to a reader. Or NULL when p_base_reader and/or single_line_begin are NULL.
*/
tetengo_json_reader_t*
tetengo_json_reader_createCommentRemovingReader(tetengo_json_reader_t* p_base_reader, const char* single_line_begin);

/*!
    \brief Creates a line counting reader.

    There is no need to destroy the base reader after calling this function.

    \param p_base_reader A pointer to a base reader.

    \return A pointer to a reader. Or NULL when p_base_reader is NULL.
*/
tetengo_json_reader_t* tetengo_json_reader_createLineCountingReader(tetengo_json_reader_t* p_base_reader);

/*!
    \brief Destroys a reader.

    \param p_reader A pointer to a reader.
*/
void tetengo_json_reader_destroy(const tetengo_json_reader_t* p_reader);

/*!
    Returns the location.

    \param p_line_counting_reader A pointer to a line counting reader.
    \param p_location             The storage for a location.

    \retval non-zero When a location has been stored.
    \retval 0        When p_line_counting_reader is NULL or not a line counting reader.
                     And/or when p_location is NULL.
                     And/or when current position is beyond the termination point.
*/
int tetengo_json_reader_getLocation(
    const tetengo_json_reader_t* p_line_counting_reader,
    tetengo_json_location_t*     p_location);

/*!
    \brief Returns non-zero when the next character exists.

    \param p_reader A pointer to a reader.

    \retval non-zero When the next character exists.
    \retval 0        Otherwise.
*/
int tetengo_json_reader_hasNext(const tetengo_json_reader_t* p_reader);

/*!
    \brief Returns the current character.

    \param p_reader A pointer to a reader.

    \return The current character. Or 0 when current position is beyond the
            termination point.
*/
char tetengo_json_reader_peek(const tetengo_json_reader_t* p_reader);

/*!
    \brief Moves to the next character.

    \param p_reader A pointer to a reader.
*/
void tetengo_json_reader_next(tetengo_json_reader_t* p_reader);

/*!
    \brief Returns the base reader.

    \param p_reader A pointer to a reader.

    \return A pointer to the base reader. Or NULL when p_reader is NULL or the reader does not have a base reader.
*/
const tetengo_json_reader_t* tetengo_json_reader_baseReader(const tetengo_json_reader_t* p_reader);


#if defined(__cplusplus)
}
#endif


#endif
