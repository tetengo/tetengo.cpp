/*! \file
    \brief A text encoder.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PLATFORMDEPENDENT_TEXTENCODER_HPP)
#define TETENGO_PLATFORMDEPENDENT_TEXTENCODER_HPP

#include <memory>
#include <string>
#include <string_view>

#include <boost/core/noncopyable.hpp>


namespace tetengo::platform_dependent
{
    /*!
        \brief A text encoder.
    */
    class text_encoder : private boost::noncopyable
    {
    public:
        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        [[nodiscard]] static const text_encoder& instance();


        // constructors and destructor

        /*!
            \brief Destroys the text encoder.
        */
        ~text_encoder();


        // functions

        /*!
            \brief Encodes a string to CP932.

            \param utf8 A string in UTF-8.

            \return A string in CP932.
        */
        [[nodiscard]] std::string encode_to_cp932(const std::string_view& utf8) const;

        /*!
            \brief Decodes a string from CP932.

            \param cp932 A string in CP932.

            \return A string in UTF-8.
        */
        [[nodiscard]] std::string decode_from_cp932(const std::string_view& cp932) const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // constructors

        text_encoder();
    };


}


#endif
