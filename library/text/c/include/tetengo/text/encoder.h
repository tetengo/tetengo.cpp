/*! \file
    \brief An encoder.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_ENCODER_H)
#define TETENGO_TEXT_ENCODER_H

#include <stddef.h>


#if defined(__cplusplus)
extern "C" {
#endif


typedef struct tetengo_text_encoder_tag tetengo_text_encoder_t;


/*! Encoding type */
typedef enum tetengo_text_encoder_encoding_tag
{
    tetengo_text_encoder_encoding_utf8, /*!< UTF-8 */
    tetengo_text_encoder_encoding_utf16, /*!< UTF-16 */
    tetengo_text_encoder_encoding_cp932 /*!< CP932 */
} tetengo_text_encoder_encoding_t;

/*!
    \brief Returns the encoder instance.

    \param encoding An encoding.

    \return A pointer to the instance. Or NULL when encoding is invalid.
*/
const tetengo_text_encoder_t* tetengo_text_encoder_instance(tetengo_text_encoder_encoding_t encoding);

/*!
    \brief Encodes a string.

    When encoding to UTF-16, cast unsigned short[] to char* for encoded_string and the unit of encoded_string_capacity
    and the return value is an unsigned short character.

    \param p_encoder               A pointer to an encoder.
    \param string                  A string.
    \param encoded_string          The storage for an encoded string. Can be NULL.
    \param encoded_string_capacity The capacity of encoded_string including a terminator '\0'.

    \return The length of encoded string. Or 0 when p_encoder is NULL or string is NULL.
*/
size_t tetengo_text_encoder_encode(
    const tetengo_text_encoder_t* p_encoder,
    const char*                   string,
    char*                         encoded_string,
    size_t                        encoded_string_capacity);

/*!
    \brief Decodes a string.

    When decoding from UTF-16, cast unsigned short[] to char* for encoded_string.

    \param p_encoder       A pointer to an encoder.
    \param encoded_string  An encoded string.
    \param string          The storage for a string. Can be NULL.
    \param string_capacity The capacity of string including a terminator '\0'.

    \return The length of decoded string. Or 0 when p_encoder is NULL or string is NULL.
*/
size_t tetengo_text_encoder_decode(
    const tetengo_text_encoder_t* p_encoder,
    const char*                   encoded_string,
    char*                         string,
    size_t                        string_capacity);


#if defined(__cplusplus)
}
#endif


#endif
