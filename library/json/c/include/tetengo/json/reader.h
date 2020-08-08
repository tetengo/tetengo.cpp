/*! \file
    \brief A reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#if !defined(TETENGO_JSON_READER_H)
#define TETENGO_JSON_READER_H

#include <stddef.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_json_reader_tag tetengo_json_reader_t;
#endif


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

    \throw std::invalid_argument When p_base_reader is nullptr or single_line_begin is empty.

    \return A pointer to a reader. Or NULL when p_base_reader and/or single_line_begin are NULL.
*/
tetengo_json_reader_t*
tetengo_json_reader_createCommentRemovingReader(tetengo_json_reader_t* p_base_reader, const char* single_line_begin);

/*!
    \brief Destroys a reader.

    \param p_reader A pointer to a reader.
*/
void tetengo_json_reader_destroy(const tetengo_json_reader_t* p_reader);

/*!
    \brief Returns non-zero when the next character exists.

    \param p_reader A pointer to a reader.

    \return non-zero When the next character exists.
    \return 0        Otherwise.
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


#if defined(__cplusplus)
}
#endif


#endif
