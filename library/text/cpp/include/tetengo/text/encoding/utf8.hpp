/*! \file
    \brief A UTF-8 encoding.

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
        \brief A UTF-8 encoding.
    */
    class utf8 : private boost::noncopyable
    {
    public:
        // types

        //! The string type.
        using string_type = std::string_view;

        //! The string view type.
        using string_view_type = std::string_view;

        //! The encoded string type.
        using encoded_string_type = std::string_view;

        //! The encoded string view type.
        using encoded_string_view_type = std::string_view;


        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        [[nodiscard]] static const utf8& instance();


        // constructors and destructor

        /*!
            \brief Destroys the encoding.
        */
        ~utf8();


        // functions

        /*!
            \brief Encodes a UTF-8 string to this encoding.

            \param utf8 A UTF-8 string.

            \return An encoded string.
        */
        [[nodiscard]] encoded_string_type encode(const string_view_type& utf8) const;

        /*!
            \brief Decodes a string in this encoding to UTF-8.

            \param string_ A string in this encoding.

            \return A decoded string.
        */
        [[nodiscard]] string_type decode(const encoded_string_view_type& string_) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        utf8();
    };


}


#endif
