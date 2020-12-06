/*! \file
    \brief An encoder.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_ENCODER_H)
#define TETENGO_TEXT_ENCODER_H


#if defined(__cplusplus)
extern "C" {
#endif


#if !defined(DOCUMENTATION)
#if !defined(TYPEDEF_TETENGO_TEXT_ENCODER_T)
#define TYPEDEF_TETENGO_TEXT_ENCODER_T
typedef struct tetengo_text_encoder_tag tetengo_text_encoder_t;
#endif
#endif


/*! Encoding type */
typedef enum tetengo_text_encoder_encoding_tag
{
    tetengo_text_encoder_encoding_utf8, /*!< UTF-8 */
    tetengo_text_encoder_encoding_utf16, /*!< UTF-16 */
    tetengo_text_encoder_encoding_cp932, /*!< CP932 */
} tetengo_text_encoder_encoding_t;

/*!
    \brief Returns the encoder instance.

    \param encoding An encoding.

    \return A pointer to the instance.
*/
const tetengo_text_encoder_t* tetengo_text_encoder_instance(tetengo_text_encoder_encoding_t encoding);


#if defined(__cplusplus)
}
#endif


#endif
