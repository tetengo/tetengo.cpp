/*! \file
    \brief A grapheme segment.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_GRAPHEMESPLITTING_GRAPHEMESEGMENT_HPP)
#define TETENGO_TEXT_GRAPHEMESPLITTING_GRAPHEMESEGMENT_HPP

#include <cstddef>
#include <memory>
#include <vector>

#include <boost/core/noncopyable.hpp>


namespace tetengo::text::grapheme_splitting
{
    /*!
        \brief A grapheme segment.
    */
    class grapheme_segment : private boost::noncopyable
    {
    public:
        // types

        //! A break peoperty type.
        enum class break_property_type
        {
            cr, //!< CR
            lf, //!< LF
            control, //!< Control
            extend, //!< Extend
            zwj, //!< ZWJ
            regional, //!< Regional
            prepend, //!< Prepend
            spacing_mark, //!< SpacingMark
            l, //!< L
            v, //!< V
            t, //!< T
            lv, //!< LV
            lvt, //!< LVT
            other, //!< Other
        };


        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        static const grapheme_segment& instance();


        // constructors and destructor

        /*!
            \brief Destroys the grapheme segment.
        */
        ~grapheme_segment();


        // functions

        /*!
            \brief Returns the grapheme offsets.

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

            \param break_properties Break properties.

            \return Grapheme offsets.
        */
        std::vector<std::size_t> segment_offsets(const std::vector<break_property_type>& break_properties) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        grapheme_segment();
    };


}


#endif
