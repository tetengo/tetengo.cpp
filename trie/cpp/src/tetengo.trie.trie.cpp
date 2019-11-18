/*! \file
    \brief A trie.

    Copyright (C) 2019 kaoru
*/

#include <memory>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/trie/trie.hpp>


namespace tetengo::trie
{
    class trie_impl::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl() : m_elements{} {};

        explicit impl(std::vector<value_type> elements) : m_elements{ std::move(elements) } {}


    private:
        // variables

        const std::vector<value_type> m_elements;
    };


    trie_impl::trie_impl() : m_p_impl{ std::make_unique<impl>() } {}

    trie_impl::trie_impl(std::vector<value_type> elements) : m_p_impl{ std::make_unique<impl>(std::move(elements)) } {}

    trie_impl::~trie_impl() = default;


}
