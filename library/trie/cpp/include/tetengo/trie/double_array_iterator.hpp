/*! \file
    \brief A double array iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
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

#include <boost/stl_interfaces/iterator_interface.hpp>


namespace tetengo::trie
{
    class storage;


    /*!
        \brief A double array iterator.
    */
    class double_array_iterator :
    public boost::stl_interfaces::iterator_interface<double_array_iterator, std ::forward_iterator_tag, std::int32_t>
    {
    public:
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


        // functions

        /*!
            \brief Dereferences the iterator.

            \return The dereferenced value.
        */
        std::int32_t& operator*();

        /*!
            \brief Dereferences the iterator.

            \return The dereferenced value.
        */
        const std::int32_t& operator*() const;

        /*!
            \brief Returns true when one iterator is equal to another.

            \param one   One iterator.
            \param another Another iterator.

            \retval true  When one is equal to another.
            \retval false Otherwise.
        */
        friend bool operator==(const double_array_iterator& one, const double_array_iterator another);

        /*!
            \brief Increments the iterator.

            \return This iterator.
        */
        double_array_iterator& operator++();

        //! Makes operator++(int) visible.
        using boost::stl_interfaces::
            iterator_interface<double_array_iterator, std ::forward_iterator_tag, std::int32_t>::operator++;

    private:
        // variables

        const storage* m_p_storage;

        std::stack<std::pair<std::size_t, std::string>, std::vector<std::pair<std::size_t, std::string>>>
            m_base_check_index_key_stack;

        std::optional<std::int32_t> m_current;


        // functions

        std::optional<std::int32_t> next();
    };


}


#endif
