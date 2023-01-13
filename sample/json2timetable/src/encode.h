/*! \file
    \brief A string encode.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#if !defined(ENCODE_H)
#define ENCODE_H

/*!
    \brief Creates an encoded string for printing.

    The return value must be freed after use.

    \param string A string.

    \retrurn A pointer to an encoded string.
*/
const char* create_encoded_for_print(const char* string);


#endif
