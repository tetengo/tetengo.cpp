/*! \file
    \brief A CP932 encoding.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_ENCODING_UTF8_HPP)
#define TETENGO_TEXT_ENCODING_UTF8_HPP

#include <memory>
#include <string_view>

#include <boost/core/noncopyable.hpp>


namespace tetengo::text::encoding
{
    /*!
        \brief A CP932 encoding.
    */
    class cp932 : private boost::noncopyable
    {
    public:
        // types

        //! The encoded string type.
        using encoded_string_type = std::string_view;


        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        [[nodiscard]] static const cp932& instance();


        // constructors and destructor

        /*!
            \brief Destroys the encoding.
        */
        ~cp932();


        // functions

        /*!
            \brief Encodes a CP932 string to this encoding.

            \param cp932 A CP932 string.

            \return An encoded string.
        */
        [[nodiscard]] encoded_string_type encode(const std::string_view& cp932) const;

        /*!
            \brief Decodes a string in this encoding to CP932.

            \param string_ A string in this encoding.

            \return A decoded string.
        */
        [[nodiscard]] std::string_view decode(const encoded_string_type& string_) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        cp932();
    };


}


#endif
