/*! \file
    \brief A stream reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_STREAMREADER_HPP)
#define TETENGO_JSON_STREAMREADER_HPP

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
        // constructors and destructor

        /*!
            \brief Creates a stream reader.

            \param p_stream        A unique pointer to an input stream.
            \param buffer_capacity A buffer capacity.

            \throw std::invalid_argument When buffer_capacity is 0.
        */
        explicit stream_reader(std::unique_ptr<std::istream>&& p_stream, std::size_t buffer_capacity = 4096);

        /*!
            \brief Destroys the stream reader.
        */
        ~stream_reader();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;


        // virtual functions

        virtual bool has_next_impl() const override;

        virtual char get_impl() const override;

        virtual void next_impl() override;
    };


}


#endif
