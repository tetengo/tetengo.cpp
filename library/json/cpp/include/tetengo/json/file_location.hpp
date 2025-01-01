/*! \file
    \brief A file location.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_FILELOCATION_HPP)
#define TETENGO_JSON_FILELOCATION_HPP

#include <cstddef>
#include <string>

#include <boost/operators.hpp>


namespace tetengo::json
{
    /*!
        \brief A file location.
    */
    class file_location : public boost::equality_comparable<file_location>
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a file location.

            \param line         A line.
            \param line_index   A line index.
            \param column_index A column index.

            \throw std::out_of_range When column_index is larger than the line length.
        */
        file_location(std::string line, std::size_t line_index, std::size_t column_index);


        // functions

        /*!
            \brief Returns true when one file location is equal to another.

            \param one   One file location.
            \param another Another file location.

            \retval true  When one is equal to another.
            \retval false Otherwise.
        */
        friend bool operator==(const file_location& one, const file_location& another);

        /*!
            \brief Returns the line.

            \return The line.
        */
        [[nodiscard]] const std::string& line() const;

        /*!
            \brief Returns the line index.

            \return The line index.
        */
        [[nodiscard]] std::size_t line_index() const;

        /*!
            \brief Returns the column index.

            \return The column index.
        */
        [[nodiscard]] std::size_t column_index() const;

        /*!
            \brief Sets a column index.

            \param index A column index.

            \throw std::out_of_range When index is larger than the line length.
        */
        void set_column_index(std::size_t index);


    private:
        // variables

        std::string m_line;

        std::size_t m_line_index;

        std::size_t m_column_index;
    };


}


#endif
