/*! \file
    \brief A reader iterato.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_READERITERATOR_HPP)
#define TETENGO_JSON_READERITERATOR_HPP

#include <cstddef>
#include <iterator>

#include <boost/stl_interfaces/iterator_interface.hpp>


namespace tetengo::json
{
    class reader;


    /*!
        \brief A reader iterator.
    */
    class reader_iterator :
    public boost::stl_interfaces::iterator_interface<reader_iterator, std::input_iterator_tag, char, char>
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates an iterator.

            It points to the last of the characters.
        */
        reader_iterator();

        /*!
            \brief Creates an iterator.

            \param reader_ A reader.
        */
        explicit reader_iterator(reader& reader_);


        // functions

        /*!
            \brief Dereferences the iterator.

            \return The dereferenced value.
        */
        [[nodiscard]] char operator*() const;

        /*!
            \brief Dereferences the iterator.

            \return The dereferenced value.
        */
        [[nodiscard]] char operator*();

        /*!
            \brief Returns true when one iterator is equal to another.

            \param one   One iterator.
            \param another Another iterator.

            \retval true  When one is equal to another.
            \retval false Otherwise.
        */
        friend bool operator==(const reader_iterator& one, const reader_iterator& another);

        /*!
            \brief Increments the iterator.

            \return This iterator.
        */
        reader_iterator& operator++();

        //! Makes operator++(int) visible.
        using boost::stl_interfaces::iterator_interface<reader_iterator, std::input_iterator_tag, char, char>::
        operator++;


    private:
        // variables

        reader* m_p_reader;

        std::size_t m_increment_count;
    };


}


#endif
