/*! \file
    \brief A comment removing reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_COMMENTREMOVINGREADER_HPP)
#define TETENGO_JSON_COMMENTREMOVINGREADER_HPP

#include <memory>
#include <string>

#include <tetengo/json/reader.hpp>


namespace tetengo::json
{
    /*!
        \brief A comment removing reader.
    */
    class comment_removing_reader : public reader
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a comment removing reader.

            \param p_base_reader     A unique pointer to a base reader.
            \param single_line_begin A beginning of a single line comment.

            \throw std::invalid_argument When p_base_reader is nullptr or single_line_begin is empty.
        */
        comment_removing_reader(std::unique_ptr<reader>&& p_base_reader, std::string single_line_begin);

        /*!
            \brief Destroys the comment removing reader.
        */
        virtual ~comment_removing_reader();


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
