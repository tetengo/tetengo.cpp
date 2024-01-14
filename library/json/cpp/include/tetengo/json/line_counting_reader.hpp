/*! \file
    \brief A line counting reader.

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_LINECOUNTINGREADER_HPP)
#define TETENGO_JSON_LINECOUNTINGREADER_HPP

#include <memory>

#include <tetengo/json/reader.hpp>


namespace tetengo::json
{
    class file_location;


    /*!
        \brief A line counting reader.
    */
    class line_counting_reader : public reader
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a line counting reader.

            \param p_base_reader A unique pointer to a base reader.

            \throw std::invalid_argument When p_base_reader is nullptr.
        */
        explicit line_counting_reader(std::unique_ptr<reader>&& p_base_reader);

        /*!
            \brief Destroys the line counting reader.
        */
        virtual ~line_counting_reader();


        // functions

        /*!
            Returns the location.

            \return The location.

            \throw std::logic_error When current position is beyond the termination point.
        */
        [[nodiscard]] const file_location& get_location() const;


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
