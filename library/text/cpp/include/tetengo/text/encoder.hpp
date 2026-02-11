/*! \file
    \brief An encoder.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_ENCODER_HPP)
#define TETENGO_TEXT_ENCODER_HPP

#include <boost/core/noncopyable.hpp>


namespace tetengo::text
{
    /*!
        \brief An encoder base.
    */
    class encoder_base : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Destroys the encoder base.
        */
        virtual ~encoder_base() = 0;
    };


    /*!
        \brief An encoder.

        \tparam Encoding An encoding type.
    */
    template <typename Encoding>
    class encoder : public encoder_base
    {
    public:
        // types

        //! The encoding type
        using encoding_type = Encoding;

        //! The string type.
        using string_type = typename encoding_type::string_type;

        //! The string view type.
        using string_view_type = typename encoding_type::string_view_type;

        //! The encoded string type.
        using encoded_string_type = typename encoding_type::encoded_string_type;

        //! The encoded string view type.
        using encoded_string_view_type = typename encoding_type::encoded_string_view_type;


        // static functions

        /*!
            \brief Returns the instance.

            \return The instance.
        */
        [[nodiscard]] static const encoder& instance()
        {
            static const encoder singleton{};
            return singleton;
        }


        // constructors and destructor

        /*!
            \brief Destroys the encoder.
        */
        virtual ~encoder() = default;


        // functions

        /*!
            \brief Encodes a string.

            \param string_ A string.

            \return An encoded string.
        */
        [[nodiscard]] encoded_string_type encode(const string_view_type& string_) const
        {
            return encoding_type::instance().encode(string_);
        }

        /*!
            \brief Decodes a string.

            \param encoded_string An encoded string.

            \return A decoded string.
        */
        [[nodiscard]] string_type decode(const encoded_string_view_type& encoded_string) const
        {
            return encoding_type::instance().decode(encoded_string);
        }
    };


}


#endif
