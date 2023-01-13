/*! \file
    \brief A stream reader.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_STREAMREADER_HPP)
#define TETENGO_JSON_STREAMREADER_HPP

#include <cstddef> // IWYU pragma: keep
#include <istream>
#include <memory>

#include <tetengo/json/reader.hpp>


namespace tetengo::json
{
    /*!
        \brief A stream reader.
    */
    class stream_reader : public reader
    {
    public:
        // static functions

        /*!
            \brief Returns the default buffer capacity.

            \return The default buffer capacity.
        */
        [[nodiscard]] static std::size_t default_buffer_capacity();


        // constructors and destructor

        /*!
            \brief Creates a stream reader.

            \param p_stream        A unique pointer to an input stream.
            \param buffer_capacity A buffer capacity.

            \throw std::invalid_argument When p_stream is nullptr or buffer_capacity is 0.
        */
        explicit stream_reader(
            std::unique_ptr<std::istream>&& p_stream,
            std::size_t                     buffer_capacity = default_buffer_capacity());

        /*!
            \brief Destroys the stream reader.
        */
        virtual ~stream_reader();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual bool has_next_impl() const override;

        virtual char peek_impl() const override;

        virtual void next_impl() override;

        virtual const reader& base_reader_impl() const override;
    };


}


#endif
