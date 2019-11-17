/*! \file
    \brief A trie.

    Copyright (C) 2019 kaoru
*/

#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/trie/trie.hpp>


namespace tetengo::trie
{
    class trie_impl::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl(){};


    private:
        // variables
    };


    trie_impl::trie_impl() : m_p_impl{ std::make_unique<impl>() } {}

    trie_impl::~trie_impl() = default;


}
