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
char tetengo_json_reader_get(const tetengo_json_reader_t* p_reader);

/*!
    \brief Moves to the next character.

    \param p_reader A pointer to a reader.
*/
void tetengo_json_reader_next(tetengo_json_reader_t* p_reader);


#if defined(__cplusplus)
}
#endif


#endif
