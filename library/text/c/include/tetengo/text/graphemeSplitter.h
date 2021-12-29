/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_GRAPHEMESPLITTER_H)
#define TETENGO_TEXT_GRAPHEMESPLITTER_H

#include <stddef.h>


#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(DOCUMENTATION)
typedef struct tetengo_text_graphemeSplitter_tag tetengo_text_graphemeSplitter_t;
#endif


/*!
    \brief A grapheme.
*/
typedef struct tetengo_text_grapheme_tag
{
    /*! An offset in the UTF-8 string. */
    size_t offset;

    /*! A width when using a monospace font. */
    size_t width;


} tetengo_text_grapheme_t;

/*!
    \brief Creates a grapheme splitter.

    Call setlocale(LC_CTYPE, [your locale]) before calling this function.
    The locale affects the result of the grapheme splitter.

    \return A pointer to a grapheme splitter.
*/
tetengo_text_graphemeSplitter_t* tetengo_text_graphemeSplitter_create();

/*!
    \brief Destroys a grapheme splitter.

    \param p_grapheme_splitter A pointer to a grapheme splitter.
*/
void tetengo_text_graphemeSplitter_destroy(const tetengo_text_graphemeSplitter_t* p_grapheme_splitter);

/*!
    \brief Split a string to graphemes.

    For emoji defined as <a href="http://www.unicode.org/reports/tr11/">"neutral" width</a>
    in the Unicode standard, this function returns a character width of 2 in the East Asian
    (Chinese, Japanese or Korean) locale, or 1 in the other locale.

    The returned graphemes include a sentinel at the tail. Its offset is the length of the string and its width is 0.

    \param p_grapheme_splitter A pointer to a grapheme splitter.
    \param string              A string.
    \param p_graphemes         The storage for graphemes. Can be NULL.

    \return A grapheme count. Or 0 when p_grapheme_splitter is NULL or string is NULL.
*/
size_t tetengo_text_graphemeSplitter_split(
    const tetengo_text_graphemeSplitter_t* p_grapheme_splitter,
    const char*                            string,
    tetengo_text_grapheme_t*               p_graphemes);


#if defined(__cplusplus)
}
#endif


#endif
