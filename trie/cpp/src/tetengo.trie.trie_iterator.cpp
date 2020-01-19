/*! \file
    \brief A trie iterator.

    Copyright (C) 2019 kaoru
*/

#include <stdexcept>

#include <tetengo/trie/trie_iterator.hpp>


namespace tetengo::trie
{
    trie_iterator::trie_iterator() {}

    std::pair<std::string, std::int32_t>& trie_iterator::dereference() const
    {
        throw std::logic_error{ "Implement it." };
    }

    bool trie_iterator::equal(const trie_iterator& /*another*/) const
    {
        throw std::logic_error{ "Implement it." };
    }

    void trie_iterator::increment()
    {
        throw std::logic_error{ "Implement it." };
    }


}
