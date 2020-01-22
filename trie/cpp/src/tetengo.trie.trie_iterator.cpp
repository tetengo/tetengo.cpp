/*! \file
    \brief A trie iterator.

    Copyright (C) 2019 kaoru
*/

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

#include <tetengo/trie/trie_iterator.hpp>


namespace tetengo::trie
{
    trie_iterator_impl::trie_iterator_impl() {}

    std::pair<std::string, std::int32_t>& trie_iterator_impl::dereference() const
    {
        throw std::logic_error{ "Implement it." };
    }

    bool trie_iterator_impl::equal(const trie_iterator_impl& /*another*/) const
    {
        throw std::logic_error{ "Implement it." };
    }

    void trie_iterator_impl::increment()
    {
        throw std::logic_error{ "Implement it." };
    }


}
