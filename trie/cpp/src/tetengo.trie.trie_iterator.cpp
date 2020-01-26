/*! \file
    \brief A trie iterator.

    Copyright (C) 2019 kaoru
*/

#include <utility>

#include <boost/iterator/iterator_facade.hpp>

#include <tetengo/trie/double_array_iterator.hpp>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/trie_iterator.hpp>


namespace tetengo::trie
{
    trie_iterator_impl::trie_iterator_impl(double_array_iterator double_array_iterator_, storage& storage_) :
    m_double_array_iterator{ std::move(double_array_iterator_) },
        m_p_storage{ &storage_ }
    {}

    trie_iterator_impl::trie_iterator_impl() : m_double_array_iterator{}, m_p_storage{ nullptr } {}

    trie_iterator_impl::mapped_type& trie_iterator_impl::dereference() const
    {
        return const_cast<mapped_type&>(*m_p_storage->mapped_at(m_double_array_iterator->second));
    }

    bool trie_iterator_impl::equal(const trie_iterator_impl& another) const
    {
        return m_double_array_iterator == another.m_double_array_iterator;
    }

    void trie_iterator_impl::increment()
    {
        ++m_double_array_iterator;
    }


}
