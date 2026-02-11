/*! \file
    \brief A trie iterator.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <utility>

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

    std::any& trie_iterator_impl::operator*()
    {
        return const_cast<std::any&>(*m_p_storage->value_at(*m_double_array_iterator));
    }

    const std::any& trie_iterator_impl::operator*() const
    {
        return *m_p_storage->value_at(*m_double_array_iterator);
    }

    bool operator==(const trie_iterator_impl& one, const trie_iterator_impl& another)
    {
        return one.m_double_array_iterator == another.m_double_array_iterator;
    }

    trie_iterator_impl& trie_iterator_impl::operator++()
    {
        ++m_double_array_iterator;
        return *this;
    }


}
