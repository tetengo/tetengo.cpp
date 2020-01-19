/*! \file
    \brief A trie iterator.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIEITERATOR_HPP)
#define TETENGO_TRIE_TRIEITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>


namespace tetengo::trie
{
    class storage;


    /*!
        \brief A trie iterator.
    */
    class trie_iterator :
    public boost::iterators::
        iterator_facade<trie_iterator, std::pair<std::string, std::int32_t>, std ::forward_iterator_tag>
    {
    public:
        // friends

        //! Allows boost::iterator_facade to access the private members.
        friend class boost::iterators::iterator_core_access;


        // constructors and destructor

        /*!
            \brief Creates an iterator.

            It points to the last of the double array.
        */
        trie_iterator();


    private:
        // functions

        std::pair<std::string, std::int32_t>& dereference() const;

        bool equal(const trie_iterator& another) const;

        void increment();
    };


}


#endif
