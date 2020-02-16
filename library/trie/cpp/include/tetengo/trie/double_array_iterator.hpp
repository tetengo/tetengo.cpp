/*! \file
    \brief A double array iterator.

    Copyright (C) 2019-2020 kaoru
*/

#if !defined(TETENGO_TRIE_DOUBLEARRAYITERATOR_HPP)
#define TETENGO_TRIE_DOUBLEARRAYITERATOR_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <optional>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>


namespace tetengo::trie
{
    class storage;


    /*!
        \brief A double array iterator.
    */
    class double_array_iterator :
    public boost::iterators::iterator_facade<double_array_iterator, std::int32_t, std ::forward_iterator_tag>
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
        double_array_iterator();

        /*!
            \brief Creates an iterator.

            \param storage_              A storage.
            \param root_base_check_index A root base-check index.
        */
        double_array_iterator(const storage& storage_, std::size_t root_base_check_index);


    private:
        // variables

        const storage* m_p_storage;

        std::stack<std::pair<std::size_t, std::string>, std::vector<std::pair<std::size_t, std::string>>>
            m_base_check_index_key_stack;

        std::optional<std::int32_t> m_current;


        // functions

        std::int32_t& dereference() const;

        bool equal(const double_array_iterator& another) const;

        void increment();

        std::optional<std::int32_t> next();
    };


}


#endif
