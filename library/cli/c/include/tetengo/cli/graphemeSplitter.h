/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_CLI_GRAPHEMESPLITTER_H)
#define TETENGO_CLI_GRAPHEMESPLITTER_H

#include <stddef.h>


#if defined(__cplusplus)
extern "C" {
#endif


#if !defined(DOCUMENTATION)
typedef struct tetengo_cli_graphemeSplitter_tag tetengo_cli_graphemeSplitter_t;
#endif


/*!
    \brief A grapheme.
*/
typedef struct tetengo_cli_grapheme_tag
{
    //! A offset in the UTF-8 string.
    size_t offset;

    //! A width when using a monospace font.
    size_t width;


} tetengo_cli_grapheme_t;

/*!
    \brief Creates a grapheme splitter.

    Call setlocale(LC_CTYPE, [your locale]) before calling this function.
    The locale affects the result of the grapheme splitter.

    \return A pointer to a grapheme splitter.
*/
tetengo_cli_graphemeSplitter_t* tetengo_cli_graphemeSplitter_create();

/*!
    \brief Destroys a grapheme splitter.

    \param p_grapheme_splitter A pointer to a grapheme splitter.
*/
void tetengo_cli_graphemeSplitter_destroy(const tetengo_cli_graphemeSplitter_t* p_grapheme_splitter);


#if defined(__cplusplus)
}
#endif


#endif
