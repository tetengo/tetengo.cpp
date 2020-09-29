/*! \file
    \brief A grapheme splitter.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_CLI_GRAPHEMESPLITTER_HPP)
#define TETENGO_CLI_GRAPHEMESPLITTER_HPP

#include <cstddef>
#include <memory>
#include <vector>

#include <boost/core/noncopyable.hpp>


namespace tetengo::cli
{
    /*!
        \brief A grapheme splitter.
    */
    class grapheme_splitter : private boost::noncopyable
    {
    public:
        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        static const grapheme_splitter& instance();


        // constructors and destructor

        /*!
            \brief Destroys the grapheme splitter.
        */
        ~grapheme_splitter();


        // functions

        /*!
            \brief Splits code points to graphemes.

            The graphemes are split between the followings.
            (/: split, X:connect, X?: connect if the number of precedings is odd)

            <pre>
             from \ to |eot|CR |LF |Con|Ext|ZWJ|Reg|Pre|SpM|L  |V  |T  |LV |LVT|Oth|
            -----------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
            sot        | / | / | / | / | / | / | / | / | / | / | / | / | / | / | / |
            CR         | / | / | X | / | / | / | / | / | / | / | / | / | / | / | / |
            LF         | / | / | / | / | / | / | / | / | / | / | / | / | / | / | / |
            Control    | / | / | / | / | / | / | / | / | / | / | / | / | / | / | / |
            Extend     | / | / | / | / | X | X | / | / | X | / | / | / | / | / | / |
            ZWJ        | / | / | / | / | X | X | / | / | X | / | / | / | / | / | X |
            Regional   | / | / | / | / | X | X | X?| / | X | / | / | / | / | / | / |
            Prepend    | / | / | / | / | X | X | X | X | X | X | X | X | X | X | X |
            SpacingMark| / | / | / | / | X | X | / | / | X | / | / | / | / | / | / |
            L          | / | / | / | / | X | X | / | / | X | X | X | / | X | X | / |
            V          | / | / | / | / | X | X | / | / | X | / | X | X | / | / | / |
            T          | / | / | / | / | X | X | / | / | X | / | / | X | / | / | / |
            LV         | / | / | / | / | X | X | / | / | X | / | X | X | / | / | / |
            LVT        | / | / | / | / | X | X | / | / | X | / | / | X | / | / | / |
            Other      | / | / | / | / | X | X | / | / | X | / | / | / | / | / | / |
            -----------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
            </pre>

            \param code_points Code points.

            \return Grapheme offsets.
        */
        std::vector<std::size_t> split(const std::vector<char32_t>& code_points) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        grapheme_splitter();
    };


}


#endif
