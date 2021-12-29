/*! \file
    \brief The usage of grapheme splitting.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include "usage_tetengo.text.graphemeSplit_c.h"

/* [grapheme_split] */
#include <assert.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#include <tetengo/text/graphemeSplitter.h>


static void save_current_locale(int category, char* storage, size_t storage_capacity);

static const char* japanese_locale_name();

static const char* english_locale_name();

void usage_tetengo_text_graphemeSplit()
{
    static const char string[] = {
        /* clang-format off */
        /* HIGH-SPEED TRAIN WITH BULLET NOSE (width: 2) */
        (char)(unsigned char)0xF0, (char)(unsigned char)0x9F, (char)(unsigned char)0x9A, (char)(unsigned char)0x85, /* U+1F685 */
        /* POLAR BEAR (width: 2) */
        (char)(unsigned char)0xF0, (char)(unsigned char)0x9F, (char)(unsigned char)0x90, (char)(unsigned char)0xBB, /* U+1F43B */
        (char)(unsigned char)0xE2, (char)(unsigned char)0x80, (char)(unsigned char)0x8D, /* U+200D */
        (char)(unsigned char)0xE2, (char)(unsigned char)0x9D, (char)(unsigned char)0x84, /* U+2744 */
        (char)(unsigned char)0xEF, (char)(unsigned char)0xB8, (char)(unsigned char)0x8F, /* U+FE0F */
        /* SNOWFLAKE (width: neutral  2 in the East Asian locale and 1 in the other locale) */
        (char)(unsigned char)0xE2, (char)(unsigned char)0x9D, (char)(unsigned char)0x84, /* U+2744 */
        0x00,
        /* clang-format on */
    };

    {
        /* Switches the locale to Japanese. */
        char initial_locale[256] = { '\0' };
        save_current_locale(LC_CTYPE, initial_locale, 256);
        if (setlocale(LC_CTYPE, japanese_locale_name()))
        {
            /* Creates a grapheme splitter for the Japanese locale. */
            const tetengo_text_graphemeSplitter_t* const p_gs = tetengo_text_graphemeSplitter_create();

            /*
                Splits the string into graphemes.
                The splitter will return three graphemes.
                The width of the grapheme of SNOWFLAKE will be 2.
            */
            const size_t grapheme_count = tetengo_text_graphemeSplitter_split(p_gs, string, NULL);
            assert(grapheme_count == 4U);
            {
                tetengo_text_grapheme_t* const p_graphemes =
                    (tetengo_text_grapheme_t*)malloc(grapheme_count * sizeof(tetengo_text_grapheme_t));
                if (p_graphemes)
                {
                    tetengo_text_graphemeSplitter_split(p_gs, string, p_graphemes);
                    assert(p_graphemes[0].offset == 0U && p_graphemes[0].width == 2U);
                    assert(p_graphemes[1].offset == 4U && p_graphemes[1].width == 2U);
                    assert(p_graphemes[2].offset == 17U && p_graphemes[2].width == 2U);
                    assert(p_graphemes[3].offset == 20U && p_graphemes[3].width == 0U);
                    free((void*)p_graphemes);
                }
            }

            /* Destroys the grapheme splitter. */
            tetengo_text_graphemeSplitter_destroy(p_gs);

            /* Restores the locale. */
            setlocale(LC_CTYPE, initial_locale);
        }
    }
    {
        /* Switches the locale to English. */
        char initial_locale[256] = { '\0' };
        save_current_locale(LC_CTYPE, initial_locale, 256);
        if (setlocale(LC_CTYPE, english_locale_name()))
        {
            /* Creates a grapheme splitter for the Japanese locale. */
            const tetengo_text_graphemeSplitter_t* const p_gs = tetengo_text_graphemeSplitter_create();

            /*
                Splits the string into graphemes.
                The splitter will return three graphemes.
                The width of the grapheme of SNOWFLAKE will be 1.
            */
            const size_t grapheme_count = tetengo_text_graphemeSplitter_split(p_gs, string, NULL);
            assert(grapheme_count == 4U);
            {
                tetengo_text_grapheme_t* const p_graphemes =
                    (tetengo_text_grapheme_t*)malloc(grapheme_count * sizeof(tetengo_text_grapheme_t));
                if (p_graphemes)
                {
                    tetengo_text_graphemeSplitter_split(p_gs, string, p_graphemes);
                    assert(p_graphemes[0].offset == 0U && p_graphemes[0].width == 2U);
                    assert(p_graphemes[1].offset == 4U && p_graphemes[1].width == 2U);
                    assert(p_graphemes[2].offset == 17U && p_graphemes[2].width == 1U);
                    assert(p_graphemes[3].offset == 20U && p_graphemes[3].width == 0U);
                    free((void*)p_graphemes);
                }
            }

            /* Destroys the grapheme splitter. */
            tetengo_text_graphemeSplitter_destroy(p_gs);

            /* Restores the locale. */
            setlocale(LC_CTYPE, initial_locale);
        }
    }
}

static void save_current_locale(const int category, char* const storage, const size_t storage_capacity)
{
    const char* const current_locale = setlocale(category, NULL);
    if (current_locale)
    {
        strncpy(storage, current_locale, storage_capacity - 1);
    }
    else
    {
        strncpy(storage, "C", storage_capacity - 1);
    }
}

static const char* japanese_locale_name()
{
#if defined(_WIN32)
    return "Japanese_Japan.932";
#else
    return "ja_JP.UTF-8";
#endif
}

static const char* english_locale_name()
{
#if defined(_WIN32)
    return "English_United States.1252";
#else
    return "en_US.UTF-8";
#endif
}
/* [grapheme_split] */
