/*! \file
    \brief A trie.

    Copyright (C) 2019 kaoru
*/

#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/trie/trie.hpp>


namespace tetengo::trie
{
    class trie::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl(){};


    private:
        // variables
    };


    trie::trie() : m_p_impl{ std::make_unique<impl>() } {}

    trie::~trie() = default;


}
