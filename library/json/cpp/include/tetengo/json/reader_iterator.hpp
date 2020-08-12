/*! \file
    \brief A reader iterato.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_JSON_READERITERATOR_HPP)
#define TETENGO_JSON_READERITERATOR_HPP

#include <cstddef>
#include <iterator>

#include <boost/iterator/iterator_facade.hpp>


namespace tetengo::json
{
    class reader;


    /*!
        \brief A reader iterator.
    */
    class reader_iterator : public boost::iterator_facade<reader_iterator, char, std::input_iterator_tag, char>
    {
    public:
        // friends

        //! Allows boost::iterator_facade to access the private members.
        friend class boost::iterators::iterator_core_access;


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


    private:
        // variables

        reader* m_p_reader;

        std::size_t m_increment_count;


        // functions

        char dereference() const;

        bool equal(const reader_iterator& another) const;

        void increment();
    };


}


#endif
