/*! \file
    \brief A trie.

    Copyright (C) 2019 kaoru
*/

#include <cstdint>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/trie.hpp>


namespace tetengo::trie
{
    class trie_impl::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl() : m_p_double_array{ std::make_unique<double_array>() } {}

        explicit impl(std::vector<value_type> elements) : m_p_double_array{}
        {
            std::vector<std::pair<std::string_view, std::int32_t>> double_array_contents{};
            double_array_contents.reserve(elements.size());
            for (auto i = static_cast<std::int32_t>(0); i < static_cast<std::int32_t>(elements.size()); ++i)
            {
                double_array_contents.emplace_back(elements[i].first, i);
            }
            m_p_double_array = std::make_unique<double_array>(double_array_contents);

        }


    private:
        // variables

        std::unique_ptr<double_array> m_p_double_array;
    };


    trie_impl::trie_impl() : m_p_impl{ std::make_unique<impl>() } {}

    trie_impl::trie_impl(std::vector<value_type> elements) : m_p_impl{ std::make_unique<impl>(std::move(elements)) } {}

    trie_impl::~trie_impl() = default;


}
