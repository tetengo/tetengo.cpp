/*! \file
    \brief A double array enumerator.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_DOUBLEARRAYENUMERATOR_HPP)
#define TETENGO_TRIE_DOUBLEARRAYENUMERATOR_HPP

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
        \brief A double array enumerator.
    */
    class double_array_enumerator :
    public boost::iterators::
        iterator_facade<double_array_enumerator, std::pair<std::string, std::int32_t>, std ::forward_iterator_tag>
    {
    public:
        // friends

        //! Allows boost::iterator_facade to access the private members.
        friend class boost::iterators::iterator_core_access;


        // constructors and destructor

        /*!
            \brief Creates an enumerator.

            It points to the last of the double array.
        */
        double_array_enumerator();

        /*!
            \brief Creates an enumerator.

            \param storage_              A storage.
            \param root_base_check_index A root base-check index.
        */
        double_array_enumerator(const storage& storage_, std::size_t root_base_check_index);


    private:
        // variables

        const storage* m_p_storage;

        std::stack<std::pair<std::size_t, std::string>, std::vector<std::pair<std::size_t, std::string>>>
            m_base_check_index_key_stack;

        std::optional<std::pair<std::string, std::int32_t>> m_current;


        // private functions

        std::pair<std::string, std::int32_t>& dereference() const;

        bool equal(const double_array_enumerator& another) const;

        void increment();

        std::optional<std::pair<std::string, std::int32_t>> next();
    };


}


#endif
